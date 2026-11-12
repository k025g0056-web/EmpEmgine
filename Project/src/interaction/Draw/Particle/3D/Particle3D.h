#pragma once
#include "Interaction/Draw/Shape/Shape.h"
#include "DataModel/ModelData.h"
#include "DataModel/TransForm3d.h"
#include "appObj/Camera/Camera3d/Camera3d.h"
#include <algorithm>
#include <vector>
#include<wrl.h>
class Particle3D :public Shape{

    ModelData   modelData_;
    ModelData   workVertices_;

    // メッシュごとに頂点バッファを持つ
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> vertexResources_;
    std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferViews_;
    std::vector<VertexData*> vertexDataPtrs_; // Map済みポインタをメッシュごとに保持

    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> meshTextureHandles_;

public:
    void SetModel(ModelData modelData) { modelData_ = modelData; }

    void Initialize(ID3D12Device* device, const ModelData& modelData,uint64_t kNumInstance);
    void Draw(
        ID3D12GraphicsCommandList* commandList,
        const Camera3d& camera,
        const Transform3d& transform,
        D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,
        const Vector4& color,int SpawnCount);

    void ReleaseModel() {
        for (auto& r : vertexResources_) {
            r.Reset();
        }

        vertexResources_.clear();

    }
};

