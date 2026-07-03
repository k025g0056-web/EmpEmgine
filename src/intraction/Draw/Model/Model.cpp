#define NOMINMAX
#include "Model.h"
#include "Internal/DX12Mechanics/DX12Mechanics.h"
#include "DataModel/Matrix4x4.h"
#include <cmath>
#include <algorithm>

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
}

void Model::Update(float dt) {
    if (!isCompressing_) return;

    timer_ += dt;

    float elapsed = timer_ - config_.delay;
    if (elapsed < 0.0f) return;

    float rawT = std::clamp(elapsed / config_.duration, 0.0f, 1.0f);

    switch (config_.easing) {
    case CompressEasing::EaseIn:
        compressionT_ = rawT * rawT;
        break;
    case CompressEasing::EaseOut:
        compressionT_ = 1.0f - (1.0f - rawT) * (1.0f - rawT);
        break;
    case CompressEasing::EaseInOut:
        compressionT_ = rawT < 0.5f
            ? 2.0f * rawT * rawT
            : 1.0f - 2.0f * (1.0f - rawT) * (1.0f - rawT);
        break;
    case CompressEasing::Bounce: {
        if (rawT < 0.8f) {
            float u = rawT / 0.8f;
            compressionT_ = u * u;
        }
        else {
            float u = (rawT - 0.8f) / 0.2f;
            compressionT_ = 1.0f + sinf(u * 3.14159f) * 0.1f;
        }
        break;
    }
    default:
        compressionT_ = rawT;
        break;
    }

    if (rawT >= 1.0f) isCompressing_ = false;

    // 変化があったフレームだけ頂点を再計算してGPUに送る
    UpdateVertices();
}

void Model::UpdateVertices()
{
    float t = compressionT_;
    if (t >= 1.0f) {
        t = 1.0f;
    }

    // スムーズ補間
    float s = 1.0f - t;
    s = s * s * (3.0f - 2.0f * s); // smoothstep

    float minY = 1e9f;
    float maxY = -1e9f;

    // ★現在のベース（重要）
    // 元データではなく「今の状態」を基準にする
    for (const auto& v : workVertices_.vertices)
    {
        minY = std::min(minY, v.position.y);
        maxY = std::max(maxY, v.position.y);
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
                pivotX + (base.position.x - pivotX) * s;
            break;
        }

        case CompressAxis::Z:
        {
            float pivotZ = 0.0f;

            dst.position.z =
                pivotZ + (base.position.z - pivotZ) * s;
            break;
        }

        default: // Y
        {
            dst.position.y =
                pivotY + (base.position.y - pivotY) * s;
            break;
        }
        }

        // ★UVは固定（歪み防止）
        dst.texcoord = base.texcoord;
    }

    std::memcpy(vertexData_,
        workVertices_.vertices.data(),
        sizeof(VertexData) * workVertices_.vertices.size());

    if (t>=1.0f) {
        t = 1.0f;
        FinishCompression();
    }
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