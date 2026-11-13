#pragma once
#include "Interaction/Draw/Shape/Shape.h"
#include "DataModel/ModelData.h"
#include "DataModel/TransForm3d.h"
#include "appObj/Camera/Camera3d/Camera3d.h"
#include <algorithm>
#include <vector>
#include<wrl.h>
#include"DataModel/PipelineSet.h"

class Particle3D :public Shape{

    ModelData   modelData_;
    ModelData   workVertices_;

    // メッシュごとに頂点バッファを持つ
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> vertexResources_;
    std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferViews_;
    std::vector<VertexData*> vertexDataPtrs_; // Map済みポインタをメッシュごとに保持

    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> meshTextureHandles_;

    uint32_t kMaxInstance = 100000;//後々10Bまで増やす

    static constexpr uint32_t kInstanceSrvIndex = 126;//SRVヒープの空き番号
    inline static ID3D12DescriptorHeap* srvHeap_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;
    TransformationMatrix* instancingData_ = nullptr;
    D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvGpu_{};

    inline static Pipeline* pipeline_ = nullptr;
public:
    void SetModel(ModelData modelData) { modelData_ = modelData; }

    void Initialize(ID3D12Device* device, const ModelData& modelData);
    void Draw(
        ID3D12GraphicsCommandList* commandList,
        const Camera3d& camera,
        const std::vector<Transform3d>& transform,
        D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,
        const Vector4& color,int SpawnCount);

    void ReleaseModel() {
        for (auto& r : vertexResources_) {
            r.Reset();
        }

        vertexResources_.clear();
    }

    static void SetSrvHeap(ID3D12DescriptorHeap* heap) { srvHeap_ = heap; }
    static void SetPipeline(Pipeline* pipeline) { pipeline_ = pipeline; }
};

