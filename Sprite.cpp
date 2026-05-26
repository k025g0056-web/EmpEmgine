#include"Sprite.h"
#include"DX12Mechanics.h"

void Sprite::Initialize(ID3D12Device* device) {
	Shape::Initialize(device);//親が先やでん
	vertexResource = DX12Mechanics::CreateBufferResource(device, sizeof(VertexData) * 6);
	vertexBufferView_ = DX12Mechanics::GenerateVertexBufferView<VertexData>(vertexResource, 6);
}

void Sprite::DrawSprite(const Transform3d& transform, const Vector2& v0, const Vector2& v1, const Vector2& v2, const Vector2& v3, ID3D12GraphicsCommandList* commandList,
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Vector4& color, const Camera3d& camera) {
	
	transform3d_ = transform;
	*wvpData = camera.GetWvpSprite(transform3d_);

	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	//左下
	vertexData[0].position = { v0.x,v0.y,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };

	//左上
	vertexData[1].position = { v1.x,v1.y,0.0f,1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };

	//右下
	vertexData[2].position = { v2.x,v2.y,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };

	//左上
	vertexData[3].position = { v1.x,v1.y,0.0f,1.0f };
	vertexData[3].texcoord = { 0.0f,0.0f };

	//右上
	vertexData[4].position = { v3.x,v3.y,0.0f,1.0f };
	vertexData[4].texcoord = { 1.0f,0.0f };

	//右下
	vertexData[5].position = { v2.x,v2.y,0.0f,1.0f };
	vertexData[5].texcoord = { 1.0f,1.0f };

	vertexResource->Unmap(0, nullptr);

	SetColor(color);

	DrawCall(commandList, vertexBufferView_, textureSrvHandleGPU, 6);

}