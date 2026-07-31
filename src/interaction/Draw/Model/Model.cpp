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
    timer_ = 0.0f;
    compressionT_ = 0.0f;
    isCompressing_ = false;

    size_t meshCount = modelData_.meshes.size();

    // ← デバッグ用に追加
    char buf[256];
    sprintf_s(buf, "MeshCount=%zu\n", meshCount);
    OutputDebugStringA(buf);
    for (size_t i = 0; i < meshCount; ++i) {
        sprintf_s(buf, "  Mesh[%zu] vertexCount=%zu materialName=%s\n",
            i, modelData_.meshes[i].vertices.size(),
            modelData_.meshes[i].materialName.c_str());
        OutputDebugStringA(buf);
    }
    vertexResources_.resize(meshCount);
    vertexBufferViews_.resize(meshCount);
    vertexDataPtrs_.resize(meshCount);

    for (size_t i = 0; i < meshCount; ++i) {
        const auto& mesh = modelData_.meshes[i];
        size_t vertexCount = mesh.vertices.size();

        // CreateBufferResourceは参照カウント1の生ポインタを返すので
        // Attach()で所有権だけ引き継ぐ(operator=だとAddRefされてリークする)
        vertexResources_[i].Attach(DX12Mechanics::CreateBufferResource(
            device, sizeof(VertexData) * vertexCount));

        // ComPtrは生ポインタへの暗黙変換を持たないのでGet()で取り出す
        vertexBufferViews_[i] = DX12Mechanics::GenerateVertexBufferView<VertexData>(
            vertexResources_[i].Get(), vertexCount);

        vertexDataPtrs_[i] = nullptr;
        vertexResources_[i]->Map(
            0, nullptr, reinterpret_cast<void**>(&vertexDataPtrs_[i]));
        std::memcpy(vertexDataPtrs_[i], mesh.vertices.data(),
            sizeof(VertexData) * vertexCount);
    }


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

    UpdateVertices();
}

void Model::UpdateVertices()
{
    float t = compressionT_;
    if (t >= 1.0f) t = 1.0f;

    float s = 1.0f - t;
    s = s * s * (3.0f - 2.0f * s); // smoothstep

    // 全メッシュ分の頂点からY範囲を求める(基準は「今の状態」)
    float minY = 1e9f;
    float maxY = -1e9f;
    for (const auto& mesh : workVertices_.meshes) {
        for (const auto& v : mesh.vertices) {
            minY = std::min(minY, v.position.y);
            maxY = std::max(maxY, v.position.y);
        }
    }
    float pivotY = minY;

    for (size_t m = 0; m < modelData_.meshes.size(); ++m) {
        const auto& baseMesh = modelData_.meshes[m];
        auto& workMesh = workVertices_.meshes[m];

        for (size_t i = 0; i < baseMesh.vertices.size(); ++i) {
            const VertexData& base = baseMesh.vertices[i];
            VertexData& dst = workMesh.vertices[i];

            dst = base;

            switch (config_.axis) {
            case CompressAxis::X: {
                float pivotX = 0.0f;
                dst.position.x = pivotX + (base.position.x - pivotX) * s;
                break;
            }
            case CompressAxis::Z: {
                float pivotZ = 0.0f;
                dst.position.z = pivotZ + (base.position.z - pivotZ) * s;
                break;
            }
            default: // Y
                dst.position.y = pivotY + (base.position.y - pivotY) * s;
                break;
            }

            dst.texCoord = base.texCoord;
        }

        // メッシュごとにGPUへ反映
        std::memcpy(vertexDataPtrs_[m],
            workMesh.vertices.data(),
            sizeof(VertexData) * workMesh.vertices.size());
    }

    if (t >= 1.0f) {
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

    wvpData->WVP = camera.GetWvp(transform3d_);
    wvpData->world = Affine(transform3d_);
    isTransformDirty_ = false;

    SetColor(color);

    // メッシュの数だけ順番にドローコールを発行する
    // (課題としてはひとまず全メッシュ同じテクスチャで描画。
    //  メッシュごとに別テクスチャを使いたい場合はここでmeshのmaterialNameから
    //  対応するSRVハンドルを引いて渡す必要がある)
    for (size_t i = 0; i < modelData_.meshes.size(); ++i) {
        DrawCallVertex(commandList, vertexBufferViews_[i],
            textureSrvHandleGPU,
            static_cast<int>(modelData_.meshes[i].vertices.size()));
    }
}

void Model::StartCompression()
{
    compressionT_ = 0.0f;
}

void Model::FinishCompression()
{
    modelData_.meshes = workVertices_.meshes; // 固定化
}