#include"ManagementTexture.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"d3d12.lib")
#include"ManagementLog.h"
#include"DX12Mechanics.h"

DirectX::ScratchImage ManagementTexture::LoadTextureFile(const std::string& filepath) {
	//テクスチャを呼んでプログラムを扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ManagementLog::ConvertString(filepath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミニマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	
	//ミニマップ付きのデータを返す
	return mipImages;
}

ID3D12Resource* ManagementTexture::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {
	//1.metadetaを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	SettingResourceByMetaData(resourceDesc, metadata);
	//2.利用するheapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	SettingHeap(heapProperties);
	//3.Resourceの精製
	ID3D12Resource* resource = nullptr;
	GenerateResource(resource, resourceDesc, heapProperties, device);
	return resource;
}

void ManagementTexture::SettingResourceByMetaData(D3D12_RESOURCE_DESC& resourceDesc,const DirectX::TexMetadata& metadata) {
	resourceDesc.Width = UINT(metadata.width);//Textureの幅
	resourceDesc.Height = UINT(metadata.height);//Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);//mipMapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);//奥行き or 配列Textureの配列数
	resourceDesc.Format = metadata.format;//TexutureのFormat
	resourceDesc.SampleDesc.Count = 1;//サンプリングカウント。１固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);//Textureの次元数。普段使っているのは二次元
}

void ManagementTexture::SettingHeap(D3D12_HEAP_PROPERTIES& heapProperties) {
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
}

void ManagementTexture::GenerateResource(ID3D12Resource*& resource, D3D12_RESOURCE_DESC& resourceDesc,
	D3D12_HEAP_PROPERTIES& heapProperties, ID3D12Device* device) {
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//HEAPの特殊な設定。特になし
		&resourceDesc,//Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,//データ転送される設計
		nullptr,//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));
}

[[nodiscard]]
ID3D12Resource* ManagementTexture::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages,
	ID3D12Device* device,ID3D12GraphicsCommandList*commandList) {
	std::vector<D3D12_SUBRESOURCE_DATA>subResources;
	DirectX::PrepareUpload(device, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subResources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subResources.size()));
	ID3D12Resource* intermediateResource = DX12Mechanics::CreateBufferResource(device, intermediateSize,WhichResource::Constant);
	UpdateSubresources(commandList, texture, intermediateResource, 0, 0, UINT(subResources.size()), subResources.data());
	//Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPYから
	//D3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

ID3D12Resource* ManagementTexture::LoadTexture(ID3D12Device* device, const std::string& filePath, 
	ID3D12GraphicsCommandList* commandList,ID3D12CommandQueue* commandQueue,
	ID3D12CommandAllocator* commandAllocator, HANDLE fenceEvent, uint64_t fenceValue, ID3D12Fence*fence) {
	//テクスチャを読んで転送する
	mipImages = LoadTextureFile(filePath);
	metadata = mipImages.GetMetadata();
	textureResource = CreateTextureResource(device, metadata);
	ID3D12Resource* intermediateResource=UploadTextureData(textureResource, mipImages,device,commandList);
	commandList->Close();

	ID3D12CommandList* commandLists[] = { commandList};
	commandQueue->ExecuteCommandLists(1, commandLists);

	commandQueue->Signal(fence, fenceValue);

	if (fence->GetCompletedValue() < fenceValue) {
		//指定したSignalにたどりついていないので、たどりつくまで待つようにイベントを設定する
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	commandAllocator->Reset();
	commandList->Reset(commandAllocator, nullptr);
	return textureResource;
}


D3D12_GPU_DESCRIPTOR_HANDLE ManagementTexture::CreateSRV(ID3D12DescriptorHeap* srvDescriptorHeap,ID3D12Device* device) {
	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dのテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	textureSrvHandleCPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの作成
	device->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);
	return textureSrvHandleGPU;
}