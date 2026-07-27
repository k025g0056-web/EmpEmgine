#pragma once
#include<d3d12.h>
#include "externals/DirectXTex/DirectXTex.h"
#include<string>
#include<vector>

struct TextureData {
	ID3D12Resource* resource = nullptr;
	DirectX::TexMetadata metadata{};
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU{};
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU{};
};

class ManagementTexture {
public:
	void Initialize(ID3D12Device* device,
	ID3D12GraphicsCommandList* commandList,
	ID3D12CommandQueue* commandQueue,
	ID3D12CommandAllocator* commandAllocator,
	HANDLE fenceEvent,
	uint64_t fenceValue,
	ID3D12Fence* fence,
	ID3D12DescriptorHeap* srvDescriptorHeap);
	D3D12_GPU_DESCRIPTOR_HANDLE Load(const std::string& filePath);
	void Release();
private:
	ID3D12Device* device_;
	ID3D12GraphicsCommandList* commandList_;
	ID3D12CommandQueue* commandQueue_;
	ID3D12CommandAllocator* commandAllocator_;
	HANDLE fenceEvent_;
	uint64_t fenceValue_;
	ID3D12Fence* fence_;
	ID3D12DescriptorHeap* srvDescriptorHeap_;

	void BuildResourceDesc(D3D12_RESOURCE_DESC& resourceDesc,const DirectX::TexMetadata& metadata);
	void SettingHeap(D3D12_HEAP_PROPERTIES& heapProperties);
	void CreateCommittedResource(ID3D12Resource*& resource, D3D12_RESOURCE_DESC& resourceDesc,D3D12_HEAP_PROPERTIES& heapProperties);
	DirectX::ScratchImage LoadTextureFile(const std::string& filePath);
	ID3D12Resource* CreateTextureResource( const DirectX::TexMetadata& metadata);
	[[nodiscard]]
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
	D3D12_SHADER_RESOURCE_VIEW_DESC BuildSrvDesc(DirectX::TexMetadata metadata);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index);
	std::vector<TextureData> textures_;
	uint32_t nextTextureIndex_ = 1; // 0番はImGui用に空ける
};
