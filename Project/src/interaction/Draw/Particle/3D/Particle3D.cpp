#define NOMINMAX
#include "Particle3D.h"
#include "Internal/DX12Mechanics/DX12Mechanics.h"
#include "DataModel/Matrix4x4.h"
#include <cmath>
#include <algorithm>

void Particle3D::Initialize(ID3D12Device* device, const ModelData& modelData,uint64_t kNumInstance) {
    Shape::Initialize(device, true);
    modelData_ = modelData;
    workVertices_ = modelData;
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
            device, sizeof(VertexData) * vertexCount*kNumInstance));

        // ComPtrは生ポインタへの暗黙変換を持たないのでGet()で取り出す
        vertexBufferViews_[i] = DX12Mechanics::GenerateVertexBufferView<VertexData>(
            vertexResources_[i].Get(), vertexCount*kNumInstance);

        vertexDataPtrs_[i] = nullptr;
        vertexResources_[i]->Map(
            0, nullptr, reinterpret_cast<void**>(&vertexDataPtrs_[i]));
        std::memcpy(vertexDataPtrs_[i], mesh.vertices.data(),
            sizeof(VertexData) * vertexCount*kNumInstance);
    }

}

void Particle3D::Draw(
    ID3D12GraphicsCommandList* commandList,
    const Camera3d& camera,
    const Transform3d& transform,
    D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,
    const Vector4& color, int SpawnCount)
{
    ChangeTransform(transform); // isTransformDirty_ 更新

    if (isTransformDirty_ || camera.IsDirty()) {
        for (int i = 0; i < modelData_.meshes.size(); i++) {
            wvpData->WVP = modelData_.meshes[i].rootNode.localMatrix * camera.GetWvp(transform3d_);
            wvpData->world = modelData_.meshes[i].rootNode.localMatrix * Affine(transform3d_);
            isTransformDirty_ = false;
        }
    }

    SetColor(color);

    SetUpCommonDrawState(commandList, textureSrvHandleGPU);

    for (size_t i = 0; i < modelData_.meshes.size(); ++i) {

        DrawCallVertexOnly(commandList, vertexBufferViews_[i],
            static_cast<int>(modelData_.meshes[i].vertices.size()), SpawnCount);
    }
}