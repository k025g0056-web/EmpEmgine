#include "Model.h"
#include"DX12Mechanics.h"
#include"Matrix4x4.h"

void Model::Initialize(ID3D12Device* device,const ModelData& modelData) {
	Shape::Initialize(device, true);//親が先やでん
	modelData_ = modelData;
	vertexResource = DX12Mechanics::CreateBufferResource(device, sizeof(VertexData) * modelData_.vertices.size());
	vertexBufferView_ = DX12Mechanics::GenerateVertexBufferView<VertexData>(vertexResource, modelData_.vertices.size());
    
    //静的データだからここに書く
    VertexData* vertexData = nullptr;
    vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
    std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
    vertexResource->Unmap(0, nullptr);
}

void Model::DrawModel(const Transform3d& transform, ID3D12GraphicsCommandList* commandList,
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Vector4& color, const Camera3d& camera) {

    ChangeTransform(transform);
    if (isTransformDirty_) {
        wvpData->WVP = camera.GetWvp(transform3d_);
        wvpData->world = Affine(transform3d_);
        isTransformDirty_ = false;
    }

    SetColor(color);

    DrawCallVertex(commandList, vertexBufferView_, textureSrvHandleGPU,
        static_cast<int>(modelData_.vertices.size()));
}