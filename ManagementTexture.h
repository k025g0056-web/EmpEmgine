#pragma once
#include <d3d11.h>
#include<d3d12.h>
#include<dxgi1_6.h>

#include <dxgi.h>
#include "externals/DirectXTex/DirectXTex.h"
#include"externals//DirectXTex/d3dx12.h"
#include<vector>
#include<string>

class ManagementTexture {
public:
	ID3D12Resource* LoadTexture(ID3D12Device* device, const std::string& filePath, 
		ID3D12GraphicsCommandList* commandList, ID3D12CommandQueue* commandQueue,ID3D12CommandAllocator* commandAllocator,
		HANDLE fenceEvent,uint64_t fenceValue, ID3D12Fence*fence);
	D3D12_GPU_DESCRIPTOR_HANDLE CreateSRV(ID3D12DescriptorHeap* srvDescriptorHeap, ID3D12Device* device);
private:
	void SettingResourceByMetaData(D3D12_RESOURCE_DESC& resourceDesc,const DirectX::TexMetadata& metadata);
	void SettingHeap(D3D12_HEAP_PROPERTIES& heapProperties);
	void GenerateResource(ID3D12Resource*& resource, D3D12_RESOURCE_DESC& resourceDesc,
		D3D12_HEAP_PROPERTIES& heapProperties, ID3D12Device* device);
	DirectX::ScratchImage LoadTextureFile(const std::string& filePath);
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	[[nodiscard]]
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages,
		ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
	ID3D12Resource* intermediateResource = nullptr;
	DirectX::ScratchImage mipImages;
	DirectX::TexMetadata metadata;

	ID3D12Resource* textureResource;
};