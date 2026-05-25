#pragma once
#include <d3d12.h>
#include<cstdint>


class DX12Mechanics {
public:
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes,size_t alignment);
	static ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);
	template<typename T>
	static D3D12_VERTEX_BUFFER_VIEW GenerateVertexBufferView(ID3D12Resource* vertexResource, size_t vertexCount) {
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		//リソースの先頭のアドレスから使う
		vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
		//使用するリソースのサイズは頂点個数分のサイズ
		vertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(T) * vertexCount);
		//一個当たりの頂点サイズ
		vertexBufferView.StrideInBytes = sizeof(T);
		return vertexBufferView;
	}
};