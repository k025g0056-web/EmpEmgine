#pragma once
#include<Windows.h>
#include <d3d12.h>
#include"Vector4.h"


class ManagementViewPort {
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	ID3D12Resource* vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Vector4* vertexData = nullptr;
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};
	
	void GenerateVertexResource(ID3D12Device* device);
	void GenerateVertexBufferView();
	void GenerateViewPort(int kWindowWidth, int kWindowHeight);
	void Write2Resource();
	void CorrectionScissorRect(int kWindowWidth, int kWindowHeight);

public:
	void Initialize(ID3D12Device* device, int kWindowWidth, int kWindowHeight);
	D3D12_VIEWPORT GetViewPort() { return viewport; }
	D3D12_RECT GetScissorRect() { return scissorRect; }
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() { return vertexBufferView; }
	void Release();
};