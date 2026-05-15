#pragma once
#include <d3d11.h>
#include<d3d12.h>
#include<dxgi1_6.h>

#include <dxgi.h>
#include "externals/DirectXTex/DirectXTex.h"
#include<string>

class ManagementTexture {
public:
	ID3D12Resource* LoadTexture(ID3D12Device* device, const std::string& filePath);
private:
	void SettingResourceByMetaData(D3D12_RESOURCE_DESC& resourceDesc,const DirectX::TexMetadata& metadata);
	void SettingHeap(D3D12_HEAP_PROPERTIES& heapProperties);
	void GenerateResource(ID3D12Resource*& resource, D3D12_RESOURCE_DESC& resourceDesc,
		D3D12_HEAP_PROPERTIES& heapProperties, ID3D12Device* device);
	DirectX::ScratchImage LoadTextureFile(const std::string& filePath);
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
};