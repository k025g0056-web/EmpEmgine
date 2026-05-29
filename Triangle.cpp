#include"Triangle.h"
#include"DX12Mechanics.h"

void Triangle::DrawTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, ID3D12GraphicsCommandList* commandList,
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,const Vector4& color) {
	ChangeVertex(v0, v1, v2);
	SetColor(color);
	if (isVertexDirty_) {
		VertexData* vertexData = nullptr;
		//書き込むためのアドレス取得
		vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
		//左下
		vertexData[0].position = { v0.x,v0.y,v0.z,1.0f };
		vertexData[0].texcoord = { 0.0f,1.0f };
		vertexData[0].normal = ToVec3(vertexData[0].position);
		
		//上
		vertexData[1].position = { v1.x,v1.y,v1.z,1.0f };
		vertexData[1].texcoord = { 0.5f,0.0f };
		vertexData[1].normal = ToVec3(vertexData[1].position);

		//右下
		vertexData[2].position = { v2.x,v2.y,v2.z,1.0f };
		vertexData[2].texcoord = { 1.0f,1.0f };
		vertexData[2].normal = ToVec3(vertexData[2].position);
		
		vertexResource->Unmap(0, nullptr);
		isVertexDirty_ = false;

	}

	DrawCall(commandList, vertexBufferView_, textureSrvHandleGPU,3);

}

void Triangle::DrawTriangle(const Transform3d& transform, const Vector3& v0, const Vector3& v1, const Vector3& v2, ID3D12GraphicsCommandList* commandList,
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Vector4& color, const Camera3d& camera) {
	ChangeTransform(transform);
	if (isTransformDirty_) {
		transform3d_ = transform;
		wvpData->WVP = camera.GetWvp(transform3d_);
		wvpData->world = Affine(transform3d_);
		isTransformDirty_ = false;
	}

	DrawTriangle(v0, v1, v2, commandList, textureSrvHandleGPU, color);
}

void Triangle::ChangeVertex(const Vector3& v0, const Vector3& v1, const Vector3& v2) {
	if ((v0==v0_)&&(v1==v1_)&&(v2==v2_)) {
		isVertexDirty_ = false;
	}else{
		isVertexDirty_ = true;
		v0_ = v0; v1_ = v1; v2_ = v2;
	}
}

void Triangle::Initialize(ID3D12Device* device) {
	Shape::Initialize(device,true);//親が先やでん
	vertexResource = DX12Mechanics::CreateBufferResource(device, sizeof(VertexData) * 3);
	vertexBufferView_ = DX12Mechanics::GenerateVertexBufferView<VertexData>(vertexResource, 3);
}