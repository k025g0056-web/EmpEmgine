#include"Sprite.h"
#include"Internal/DX12Mechanics/DX12Mechanics.h"

void Sprite::Initialize(ID3D12Device* device) {
	Shape::Initialize(device,false);//親が先やでん
	vertexResource = DX12Mechanics::CreateBufferResource(device, sizeof(VertexData) * 4);
	vertexBufferView_ = DX12Mechanics::GenerateVertexBufferView<VertexData>(vertexResource, 4);
	indexResource_ = DX12Mechanics::CreateBufferResource(device, sizeof(uint32_t) * 6);
	indexBufferView_ = DX12Mechanics::GenerateIndexBufferView<uint32_t>(indexResource_,6);
}

void Sprite::Draw(ID3D12GraphicsCommandList* commandList, const Camera3d& camera, const Transform3d& transform,
	const Vector2& v0, const Vector2& v1, const Vector2& v2, const Vector2& v3,
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Vector4& color,const Transform3d& uvTransform) {
	
	transform3d_ = transform;
	wvpData->WVP = camera.GetWvpSprite(transform3d_);
	wvpData->world = Affine(transform3d_);

	
	SetUvTransForm(uvTransform);

	uint32_t* indexData = nullptr;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;

	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	//左下
	vertexData[0].position = { v0.x,v0.y,0.0f,1.0f };
	vertexData[0].texCoord = { 0.0f,1.0f };
	vertexData[0].normal = ToVec3(vertexData[0].position);

	//左上
	vertexData[1].position = { v1.x,v1.y,0.0f,1.0f };
	vertexData[1].texCoord = { 0.0f,0.0f };
	vertexData[1].normal = ToVec3(vertexData[1].position);

	//右下
	vertexData[2].position = { v2.x,v2.y,0.0f,1.0f };
	vertexData[2].texCoord = { 1.0f,1.0f };
	vertexData[2].normal = ToVec3(vertexData[2].position);

	//左上
	vertexData[3].position = { v3.x,v3.y,0.0f,1.0f };
	vertexData[3].texCoord = { 1.0f,0.0f };
	vertexData[3].normal = ToVec3(vertexData[3].position);

	vertexResource->Unmap(0, nullptr);

	SetColor(color);

	DrawCallIndex(commandList,indexBufferView_, vertexBufferView_, textureSrvHandleGPU, 6);

}