#pragma once
#include <d3d12.h>
#include<cstdint>
#include <type_traits>

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

	template<typename T>
	static D3D12_INDEX_BUFFER_VIEW GenerateIndexBufferView(ID3D12Resource* indexResource, size_t indexCount) {
		
		// uint16_t or uint32_tを型で自動判定
		static_assert(
			std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t>,
			"IndexBuffer の型は uint16_t か uint32_t だけ");

		constexpr DXGI_FORMAT format =std::is_same_v<T, uint16_t>? DXGI_FORMAT_R16_UINT: DXGI_FORMAT_R32_UINT;


		D3D12_INDEX_BUFFER_VIEW indexBufferView{};
		//リソースの先頭のアドレスから使う
		indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
		//使用するリソースのサイズは頂点個数分のサイズ
		indexBufferView.SizeInBytes = static_cast<UINT>(sizeof(T) * indexCount);
		//インデックスはuint32_tとする
		indexBufferView.Format = format;
		return indexBufferView;
	}
};