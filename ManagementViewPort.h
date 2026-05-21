#pragma once
#include<Windows.h>
#include <d3d12.h>
#include"Vector4.h"
#include"Matrix4x4.h"
#include"TransForm3d.h"
#include"Camera.h"
#include"VertexData.h"
#include<cstdint>

class ManagementViewPort {
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	ID3D12Resource* vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};
	ID3D12Resource* materialResource;
	Vector4* materialData = nullptr;
	ID3D12Resource* wvpResource;
	Matrix4x4* wvpData = nullptr;
	Transform3d transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	VertexData* vertexData_ = nullptr;

	void GenerateVertexResource(ID3D12Device* device);
	void GenerateVertexBufferView();
	void GenerateViewPort(int kWindowWidth, int kWindowHeight);
	void Write2Resource();
	void CorrectionScissorRect(int kWindowWidth, int kWindowHeight);
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
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
	void Update(Camera camera);
	void Release();
};