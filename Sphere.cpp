#include"Sphere.h"
#include"DX12Mechanics.h"
#include<cstdint>
#include<numbers>
#include<cmath>

void Sphere::Initialize(ID3D12Device* device) {
	Shape::Initialize(device,true);//親が先やでん
	const uint32_t vertexCount = kSubdivision * kSubdivision * 6;
	vertexResource = DX12Mechanics::CreateBufferResource(device, sizeof(VertexData) * vertexCount);
	vertexBufferView_ = DX12Mechanics::GenerateVertexBufferView<VertexData>(vertexResource, vertexCount);
}

void Sphere::DrawSphere(const Transform3d& transform, const Vector4& color, ID3D12GraphicsCommandList* commandList,
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Camera3d& camera) {
	transform3d_ = transform;
	wvpData->WVP = camera.GetWvp(transform3d_);
	wvpData->world = Affine(transform3d_);
	SetColor(color);
	VertexData* vertexData = nullptr;
	//書き込むためのアドレス取得
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
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;//φ
			
			//texCo.x
			float u0 = static_cast<float>(lonIndex) / kSubdivision;
			float u1 = static_cast<float>(lonIndex + 1) / kSubdivision;

			//texCo.y
			float v0 = 1.0f - static_cast<float>(latIndex) / kSubdivision;
			float v1 = 1.0f - static_cast<float>(latIndex + 1) / kSubdivision;

			//頂点にデータを入力するa(左下)
			vertexData[start].position.x = std::cos(lat) * std::cos(lon);
			vertexData[start].position.y = std::sin(lat);
			vertexData[start].position.z = std::cos(lat) * std::sin(lon);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texcoord = { u0,v0 };
			vertexData[start].normal = ToVec3(vertexData[start].position);

			//左上
			vertexData[start + 1].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[start + 1].position.y = std::sin(lat + kLatEvery);
			vertexData[start + 1].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[start + 1].position.w = 1.0f;
			vertexData[start+1].texcoord = { u0,v1 };
			vertexData[start + 1].normal = ToVec3(vertexData[start + 1].position);

			//右下
			vertexData[start + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[start + 2].position.y = std::sin(lat);
			vertexData[start + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[start + 2].position.w = 1.0f;
			vertexData[start + 2].texcoord = { u1,v0 };
			vertexData[start + 2].normal = ToVec3(vertexData[start + 2].position);

			//左上
			vertexData[start + 3].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[start + 3].position.y = std::sin(lat + kLatEvery);
			vertexData[start + 3].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[start + 3].position.w = 1.0f;
			vertexData[start + 3].texcoord = { u0,v1 };
			vertexData[start + 3].normal = ToVec3(vertexData[start + 3].position);

			//右上
			vertexData[start + 4].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertexData[start + 4].position.y = std::sin(lat + kLatEvery);
			vertexData[start + 4].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertexData[start + 4].position.w = 1.0f;
			vertexData[start + 4].texcoord = { u1,v1 };
			vertexData[start + 4].normal = ToVec3(vertexData[start + 4].position);

			//右下
			vertexData[start + 5].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[start + 5].position.y = std::sin(lat);
			vertexData[start + 5].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[start + 5].position.w = 1.0f;
			vertexData[start + 5].texcoord = { u1,v0 };
			vertexData[start + 5].normal = ToVec3(vertexData[start + 5].position);

		}
	}

	vertexResource->Unmap(0, nullptr);
	DrawCall(commandList, vertexBufferView_, textureSrvHandleGPU, kSubdivision * kSubdivision * 6);
}