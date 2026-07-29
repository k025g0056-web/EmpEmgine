#define NOMINMAX
#include "Model.h"
#include "Internal/DX12Mechanics/DX12Mechanics.h"
#include "DataModel/Matrix4x4.h"
#include <cmath>
#include <algorithm>

namespace {

    // rawT(0~1)をeasingの種類に応じて変換する。
    // Bounceだけは意図的に1.0を一瞬超えてから戻る（潰れすぎてから戻るバネ挙動）。
    float ApplyEasing(float rawT, CompressEasing easing) {
        switch (easing) {
        case CompressEasing::EaseIn:
            return rawT * rawT;

        case CompressEasing::EaseOut:
            return 1.0f - (1.0f - rawT) * (1.0f - rawT);

        case CompressEasing::EaseInOut:
            return rawT < 0.5f
                ? 2.0f * rawT * rawT
                : 1.0f - 2.0f * (1.0f - rawT) * (1.0f - rawT);

        case CompressEasing::Bounce: {
            if (rawT < 0.8f) {
                float u = rawT / 0.8f;
                return u * u;
            }
            float u = (rawT - 0.8f) / 0.2f;
            return 1.0f + sinf(u * 3.14159f) * 0.1f;
        }

        default: // Linear
            return rawT;
        }
    }

    // 重さ(kg想定)から「最終的にどこまで潰れるか(0~1、1で完全に潰れる)」を求める。
    // 300kgでほぼ潰れきる想定。数値は感覚で決めているので、体感で調整してよい。
    float WeightToMaxCompression(float weightKg) {
        constexpr float kMaxWeight = 300.0f;
        constexpr float kMinCompression = 0.1f;
        constexpr float kMaxCompression = 0.85f;

        float t = std::clamp(weightKg / kMaxWeight, 0.0f, 1.0f);
        t = t * t;
        return kMinCompression + (kMaxCompression - kMinCompression) * t;
    }

} // namespace

void Model::Initialize(ID3D12Device* device, const ModelData& modelData) {
    Shape::Initialize(device, true);
    modelData_ = modelData;
    workVertices_ = modelData;
    timer_ = 0.0f;        // ← 追加
    compressionT_ = 0.0f;        // ← 追加
    isCompressing_ = false;      // ← 追加

    vertexResource = DX12Mechanics::CreateBufferResource(
        device, sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_ = DX12Mechanics::GenerateVertexBufferView<VertexData>(
        vertexResource, modelData_.vertices.size());

    // ← vertexData_を必ず取り直すなの！
    vertexData_ = nullptr;
    vertexResource->Map(
        0, nullptr, reinterpret_cast<void**>(&vertexData_));
    std::memcpy(vertexData_, modelData_.vertices.data(),
        sizeof(VertexData) * modelData_.vertices.size());
}
void Model::StartCompress(const CompressConfig& config) {
    config_ = config;
    timer_ = 0.0f;
    compressionT_ = 0.0f;
    isCompressing_ = true;
    isRecovering_ = false;
    recoverTimer_ = 0.0f;
}

void Model::Update(float dt)
{
    if (isCompressing_)
    {
        timer_ += dt;

        float elapsed = timer_ - config_.delay;
        if (elapsed >= 0.0f)
        {
            float rawT = std::clamp(elapsed / config_.duration, 0.0f, 1.0f);
            compressionT_ = ApplyEasing(rawT, config_.easing);

            UpdateVertices();

            if (rawT >= 1.0f)
            {
                compressionT_ = 1.0f;   // ★追加

                isCompressing_ = false;
                isRecovering_ = true;
                recoverTimer_ = recoverWait_;
            }
        }
    }

    else if (isRecovering_)
    {
        recoverTimer_ -= dt;

        if (recoverTimer_ <= 0.0f)
        {
            compressionT_ -= dt / config_.duration;

            if (compressionT_ <= 0.0f)
            {
                compressionT_ = 0.0f;
                isRecovering_ = false;
            }

            UpdateVertices();
        }
    }
}

void Model::UpdateVertices()
{
    // ★compressionT_はクランプしない。
    //   Bounceイージングがここで1.0を超えないと、潰れすぎてから戻る演出が消えてしまう。
    float s = 1.0f - compressionT_;
    s = s * s * (3.0f - 2.0f * s); // smoothstep

    // 重さに応じて「最終的にどこまで潰れるか」を決め、sをその範囲に写像する。
    // squashed = 1.0で無変形、0.0で最大まで潰れた状態。
    float maxCompression = WeightToMaxCompression(config_.weight);
    float squashed = 1.0f - (1.0f - s) * maxCompression;

    // 潰れた分だけ横に膨らませて、体積が保たれているように見せる。
    float bulge = 1.0f + (1.0f - squashed) * 0.8f;

    float minY = 1e9f;

    // ★現在のベース（重要）
    // 元データではなく「今の状態」を基準にする
    for (const auto& v : workVertices_.vertices)
    {
        minY = std::min(minY, v.position.y);
    }

    float pivotY = minY;

    for (size_t i = 0; i < modelData_.vertices.size(); i++)
    {
        const VertexData& base = modelData_.vertices[i];
        VertexData& dst = workVertices_.vertices[i];

        // ★ここが重要：毎回リセットしない
        dst = base;

        switch (config_.axis)
        {
        case CompressAxis::X:
        {
            float pivotX = 0.0f;

            dst.position.x =
                pivotX + (base.position.x - pivotX) * squashed;
            dst.position.y = base.position.y * bulge;
            dst.position.z = base.position.z * bulge;
            break;
        }

        case CompressAxis::Z:
        {
            float pivotZ = 0.0f;

            dst.position.z =
                pivotZ + (base.position.z - pivotZ) * squashed;
            dst.position.x = base.position.x * bulge;
            dst.position.y = base.position.y * bulge;
            break;
        }

        default: // Y
        {
            dst.position.y =
                pivotY + (base.position.y - pivotY) * squashed;
            dst.position.x = base.position.x * bulge;
            dst.position.z = base.position.z * bulge;
            break;
        }
        }

        // ★UVは固定（歪み防止）
        dst.texcoord = base.texcoord;
    }

    std::memcpy(vertexData_,
        workVertices_.vertices.data(),
        sizeof(VertexData) * workVertices_.vertices.size());

    // ★完了判定はisCompressing_で行う（rawT基準）。
    //   compressionT_をクランプして判定すると、Bounceのオーバーシュート中に
    //   毎フレームFinishCompression()が呼ばれてしまうので注意。
   // if (!isCompressing_) {
      //  FinishCompression();
    //}
}


void Model::DrawModel(
    const Transform3d& transform,
    ID3D12GraphicsCommandList* commandList,
    D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,
    const Vector4& color,
    const Camera3d& camera)
{
    ChangeTransform(transform);

    // ← 毎フレーム必ず更新するなの！isTransformDirty_チェックを外す
    wvpData->WVP = camera.GetWvp(transform3d_);
    wvpData->world = Affine(transform3d_);
    isTransformDirty_ = false;

    SetColor(color);

    DrawCallVertex(commandList, vertexBufferView_,
        textureSrvHandleGPU,
        static_cast<int>(modelData_.vertices.size()));
}

void Model::StartCompression()
{
    compressionT_ = 0.0f;
}

void Model::FinishCompression()
{
    modelData_.vertices = workVertices_.vertices; // 固定化
}