#include"DoubleTriangle.h"
#include"DX12Mechanics.h"
void DoubleTriangle::DrawDoubleTriangle(const Transform3d&transform, ID3D12GraphicsCommandList* commandList,
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,const Camera3d& camera) {
	ChangeTransform(transform);
	if (isTransformDirty_) {
		transform3d_ = transform;
		*wvpData = camera.GetWvp(transform3d_);
		isTransformDirty_ = false;
	}

	VertexData* vertexData = nullptr;
	//書き込むためのアドレス取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal = ToVec3(vertexData[0].position);

	//上
	vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.5f,0.0f };
	vertexData[1].normal = ToVec3(vertexData[1].position);

	//右下
	vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[2].normal = ToVec3(vertexData[2].position);

	//左下
	vertexData[3].position = { -0.5f,-0.5f,0.5f,1.0f };
	vertexData[3].texcoord = { 0.0f,1.0f };
	vertexData[3].normal = ToVec3(vertexData[3].position);

	//上
	vertexData[4].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData[4].texcoord = { 0.5f,0.0f };
	vertexData[4].normal = ToVec3(vertexData[4].position);

	//右下
	vertexData[5].position = { 0.5f,-0.5f,-0.5f,1.0f };
	vertexData[5].texcoord = { 1.0f,1.0f };
	vertexData[4].normal = ToVec3(vertexData[4].position);

	vertexResource->Unmap(0, nullptr);

	DrawCall(commandList, vertexBufferView_, textureSrvHandleGPU, 6);
}

void DoubleTriangle::Initialize(ID3D12Device* device) {
	Shape::Initialize(device,true);
	vertexResource = DX12Mechanics::CreateBufferResource(device, sizeof(VertexData) * 6);
	vertexBufferView_ = DX12Mechanics::GenerateVertexBufferView<VertexData>(vertexResource, 6);
}