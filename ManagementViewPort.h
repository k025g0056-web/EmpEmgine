#pragma once
#include<Windows.h>
#include <d3d12.h>
#include"Vector4.h"
#include"Matrix4x4.h"
#include"TransForm3d.h"
#include"Camera3d.h"
#include"VertexData.h"
#include<cstdint>
#include"DX12Mechanics.h"

class ManagementViewPort {

	ID3D12Resource* vertexResource = nullptr;
	ID3D12Resource* materialResource = nullptr;
	Vector4* materialData = nullptr;
	ID3D12Resource* wvpResource = nullptr;
	Matrix4x4* wvpData = nullptr;
	VertexData* vertexData_ = nullptr;
	ID3D12Resource* depthStencilResource_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};
	Transform3d transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	void GenerateVertexBufferView();
	void GenerateViewPort(int kWindowWidth, int kWindowHeight);
	void Write2Resource();
	void CorrectionScissorRect(int kWindowWidth, int kWindowHeight);
	void GenerateMaterial(ID3D12Device* device);
	void GenerateWvpResource(ID3D12Device* device);
	ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);
public:
	void Initialize(ID3D12Device* device, int kWindowWidth, int kWindowHeight);
	D3D12_VIEWPORT GetViewPort() { return viewport; }
	D3D12_RECT GetScissorRect() { return scissorRect; }
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() { return vertexBufferView; }
	ID3D12Resource* GetMaterialResource() { return materialResource; }
	ID3D12Resource* GetWvpResource() { return wvpResource; }
	Transform3d GetTransform() { return transform; }
	ID3D12Resource* GetDepthStencilResource() { return depthStencilResource_; }
	void Update(Camera3d camera);
	void Release();
};