#include"Sphere.h"
#include"Internal/DX12Mechanics/DX12Mechanics.h"
#include<cstdint>
#include<numbers>
#include<cmath>

void Sphere::Initialize(ID3D12Device* device) {
	Shape::Initialize(device,true);//親が先やでん
	const uint32_t vertexCount = kSubdivision * kSubdivision;
	vertexResource = DX12Mechanics::CreateBufferResource(device, sizeof(VertexData) * vertexCount*4);
	vertexBufferView_ = DX12Mechanics::GenerateVertexBufferView<VertexData>(vertexResource, vertexCount*4);
	indexResource_ = DX12Mechanics::CreateBufferResource(device, sizeof(uint32_t) * vertexCount*6);
	indexBufferView_ = DX12Mechanics::GenerateIndexBufferView<UINT>(indexResource_, vertexCount*6);
}

void Sphere::DrawSphere(const Transform3d& transform, const Vector4& color, ID3D12GraphicsCommandList* commandList,
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Camera3d& camera) {
	transform3d_ = transform;
	wvpData->WVP = camera.GetWvp(transform3d_);
	wvpData->world = Affine(transform3d_);
	SetColor(color);
	uint32_t* indexData = nullptr;
	VertexData* vertexData = nullptr;
	//書き込むためのアドレス取得
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//経度分割一つ分の角度(φの元)
	const float kLonEvery = std::numbers::pi_v<float>*2.0f / static_cast<float>(kSubdivision);
	//緯度一つ分の角度(θの元)
	const float kLatEvery= std::numbers::pi_v<float> / static_cast<float>(kSubdivision);

	//緯度の方向に分割
	for (int latIndex = 0; latIndex < kSubdivision;++latIndex) {
		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;//θ
		//経度の方向に分割しながら線を描く
		for (int lonIndex = 0; lonIndex < kSubdivision;++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 4;
			float lon = lonIndex * kLonEvery;//φ
			uint32_t iStart = (latIndex * kSubdivision + lonIndex) * 6;
			//texCo.x
			float u0 = static_cast<float>(lonIndex) / kSubdivision;
			float u1 = static_cast<float>(lonIndex + 1) / kSubdivision;

			//texCo.y
			float v0 = 1.0f - static_cast<float>(latIndex) / kSubdivision;
			float v1 = 1.0f - static_cast<float>(latIndex + 1) / kSubdivision;

			indexData[iStart] = start;
			indexData[iStart + 1] = start + 1;
			indexData[iStart + 2] = start + 2;
			indexData[iStart + 3] = start + 1;
			indexData[iStart + 4] = start + 3;
			indexData[iStart + 5] = start + 2;

			//頂点にデータを入力するa(左下)
			vertexData[start].position.x = std::cos(lat) * std::cos(lon);
			vertexData[start].position.y = std::sin(lat);
			vertexData[start].position.z = std::cos(lat) * std::sin(lon);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texCoord = { u0,v0 };
			vertexData[start].normal = ToVec3(vertexData[start].position);

			//左上
			vertexData[start + 1].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[start + 1].position.y = std::sin(lat + kLatEvery);
			vertexData[start + 1].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[start + 1].position.w = 1.0f;
			vertexData[start+1].texCoord = { u0,v1 };
			vertexData[start + 1].normal = ToVec3(vertexData[start + 1].position);

			//右下
			vertexData[start + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[start + 2].position.y = std::sin(lat);
			vertexData[start + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[start + 2].position.w = 1.0f;
			vertexData[start + 2].texCoord = { u1,v0 };
			vertexData[start + 2].normal = ToVec3(vertexData[start + 2].position);

			// 右上
			vertexData[start + 3].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertexData[start + 3].position.y = std::sin(lat + kLatEvery);
			vertexData[start + 3].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertexData[start + 3].position.w = 1.0f;
			vertexData[start + 3].texCoord = { u1, v1 };
			vertexData[start + 3].normal = ToVec3(vertexData[start + 3].position);
		}
	}

	vertexResource->Unmap(0, nullptr);
	DrawCallIndex(commandList,indexBufferView_, vertexBufferView_, textureSrvHandleGPU, kSubdivision * kSubdivision * 6);
}