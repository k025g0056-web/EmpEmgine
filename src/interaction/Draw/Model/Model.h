#pragma once
#include "Interaction/Draw/Shape/Shape.h"
#include "DataModel/ModelData.h"
#include "DataModel/TransForm3d.h"
#include "appObj/Camera/Camera3d/Camera3d.h"
#include <algorithm>
#include <vector>
#include<wrl.h>

enum class CompressAxis {
    X, Y, Z,
};

enum class CompressEasing {
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    Bounce,
};

struct CompressConfig {
    CompressAxis   axis = CompressAxis::Y;
    CompressEasing easing = CompressEasing::Linear;
    float          duration = 1.0f;
    float          delay = 0.0f;
};

class Model : public Shape {
    ModelData   modelData_;
    ModelData   workVertices_;

    // メッシュごとに頂点バッファを持つ
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> vertexResources_;
    std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferViews_;
    std::vector<VertexData*> vertexDataPtrs_; // Map済みポインタをメッシュごとに保持

    float       timer_ = 0.0f;
    float       compressionT_ = 0.0f;
    CompressConfig config_;
    bool        isCompressing_ = false;

public:
    void SetModel(ModelData modelData) { modelData_ = modelData; }

    void Initialize(ID3D12Device* device, const ModelData& modelData);
    void StartCompress(const CompressConfig& config);
    bool IsFinished() const { return compressionT_ >= 1.0f; }
    void Update(float dt);
    void UpdateVertices();
    void Draw(
        ID3D12GraphicsCommandList* commandList,
        const Camera3d& camera,
        const Transform3d& transform,
        D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,
        const Vector4& color);
    void StartCompression();
    void FinishCompression();
};