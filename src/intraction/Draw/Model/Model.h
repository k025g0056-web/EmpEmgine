#pragma once
#include "Intraction/Draw/Shape/Shape.h"
#include "DataModel/ModelData.h"
#include "DataModel/TransForm3d.h"
#include "appObj/Camera/Camera3d/Camera3d.h"
#include <algorithm>

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
    float          weight = 1.0f; // kg見立て。潰れの最大量とふくらみ量を決める
};

class Model : public Shape {
    ModelData   modelData_;
    ModelData   workVertices_;
    VertexData* vertexData_ = nullptr;
    float       timer_ = 0.0f;
    float       compressionT_ = 0.0f;
    CompressConfig config_;
    bool        isCompressing_ = false;
    bool isRecovering_ = false;
    float recoverTimer_ = 0.0f;
    float recoverWait_ = 0.5f;

public:
    void Initialize(ID3D12Device* device, const ModelData& modelData);
    void StartCompress(const CompressConfig& config);
    bool IsFinished() const { return compressionT_ >= 1.0f; }
    bool IsCompressing() const { return isCompressing_; } // 潰れアニメーション中かどうか（多重発動の防止用）
    void Update(float dt);
    void UpdateVertices();
    void DrawModel(
        const Transform3d& transform,
        ID3D12GraphicsCommandList* commandList,
        D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,
        const Vector4& color,
        const Camera3d& camera);
    void StartCompression();
    void FinishCompression();

};