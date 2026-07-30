#include"ManagementTexture.h"
#pragma comment(lib,"d3d12.lib")
#include"Log/ManagementLog.h"
#include"Internal/DX12Mechanics/DX12Mechanics.h"
#include"externals//DirectXTex/d3dx12.h"

void ManagementTexture::Initialize(ID3D12Device* device,ID3D12GraphicsCommandList* commandList,
	ID3D12CommandQueue* commandQueue,ID3D12CommandAllocator* commandAllocator,HANDLE fenceEvent,
	uint64_t fenceValue,ID3D12Fence* fence,ID3D12DescriptorHeap* srvDescriptorHeap) {
	device_ = device;
	commandList_ = commandList;
	commandQueue_ = commandQueue;
	commandAllocator_ = commandAllocator;
	fenceEvent_ = fenceEvent;
	fence_ = fence;
	fenceValue_ = fenceValue;
	srvDescriptorHeap_ = srvDescriptorHeap;
}

DirectX::ScratchImage ManagementTexture::LoadTextureFile(const std::string& filepath) {

	// 落ちた時にファイルの名前を出力する
	wchar_t currentDir[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, currentDir);
	OutputDebugStringW(L"=== LoadTextureFile ===\n");
	OutputDebugStringW(L"カレントディレクトリ: ");
	OutputDebugStringW(currentDir);
	OutputDebugStringW(L"\n読もうとしてるファイル: ");
	OutputDebugStringW(ManagementLog::ConvertString(filepath).c_str());
	OutputDebugStringW(L"\n");


	DirectX::ScratchImage image{};
	std::wstring filePathW = ManagementLog::ConvertString("resources/Image/" + filepath);

	// 拡張子を取得する
	std::wstring ext = filePathW.substr(filePathW.find_last_of(L'.'));
	// 小文字に変換する
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	HRESULT hr;

	if (ext == L".hdr" || ext == L".exr") {
		// HDR系はWICじゃなくてこっち
		hr = DirectX::LoadFromHDRFile(filePathW.c_str(), nullptr, image);
	}
	else {
		// jpgのときはFORCE_SRGBを外す！
		DirectX::WIC_FLAGS flag = (ext == L".jpg" || ext == L".jpeg")
			? DirectX::WIC_FLAGS_NONE
			: DirectX::WIC_FLAGS_FORCE_SRGB;

		hr = DirectX::LoadFromWICFile(filePathW.c_str(), flag, nullptr, image);
	}

	assert(SUCCEEDED(hr));

	//サイズが1の時はこれ以上小さくしない
	const DirectX::TexMetadata& metadata = image.GetMetadata();
	if (metadata.width == 1 && metadata.height == 1) {
		return image;
	}

	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	return mipImages;
}

ID3D12Resource* ManagementTexture::CreateTextureResource(const DirectX::TexMetadata& metadata) {
	//1.metadetaを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	BuildResourceDesc(resourceDesc, metadata);
	//2.利用するheapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	SettingHeap(heapProperties);
	//3.Resourceの精製
	ID3D12Resource* resource = nullptr;
	CreateCommittedResource(resource, resourceDesc, heapProperties);
	return resource;
}

void ManagementTexture::BuildResourceDesc(
	D3D12_RESOURCE_DESC& resourceDesc,
	const DirectX::TexMetadata& metadata)
{
	resourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	resourceDesc.Alignment = 0;
	resourceDesc.Width = metadata.width;
	resourceDesc.Height = static_cast<UINT>(metadata.height);
	resourceDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
	resourceDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
}

void ManagementTexture::SettingHeap(D3D12_HEAP_PROPERTIES& heapProperties) {
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
}

void ManagementTexture::CreateCommittedResource(ID3D12Resource*& resource, D3D12_RESOURCE_DESC& resourceDesc,
	D3D12_HEAP_PROPERTIES& heapProperties) {

	HRESULT hr = device_->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource));

	assert(SUCCEEDED(hr));
}

[[nodiscard]]
ID3D12Resource* ManagementTexture::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {
	std::vector<D3D12_SUBRESOURCE_DATA>subResources;
	DirectX::PrepareUpload(device_, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subResources);
	
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subResources.size()));
	ID3D12Resource* intermediateResource = DX12Mechanics::CreateBufferResource(device_, intermediateSize);
	UpdateSubresources(commandList_, texture, intermediateResource, 0, 0, UINT(subResources.size()), subResources.data());
	//Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPYから
	//D3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList_->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

D3D12_GPU_DESCRIPTOR_HANDLE ManagementTexture::Load( const std::string& filePath) {
	DirectX::ScratchImage mipImages;
	DirectX::TexMetadata metadata;
	ID3D12Resource* textureResource = nullptr;
	//テクスチャを読んで転送する
	mipImages = LoadTextureFile(filePath);
	metadata = mipImages.GetMetadata();
	textureResource = CreateTextureResource(metadata);
	ID3D12Resource* intermediateResource=UploadTextureData(textureResource, mipImages);
	
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = BuildSrvDesc(metadata);
	const uint32_t descriptorSizeSRV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	const uint32_t descriptorSizeRTV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	const uint32_t descriptorSizeDSV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = GetCPUDescriptorHandle(srvDescriptorHeap_, descriptorSizeSRV, nextTextureIndex_);
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = GetGPUDescriptorHandle(srvDescriptorHeap_, descriptorSizeSRV, nextTextureIndex_);
	device_->CreateShaderResourceView(textureResource, &srvDesc, handleCPU);
	nextTextureIndex_++;

	//リストを閉じる
	commandList_->Close();

	ID3D12CommandList* commandLists[] = { commandList_};
	commandQueue_->ExecuteCommandLists(1, commandLists);
	fenceValue_++;
	//シグナルを送る
	commandQueue_->Signal(fence_, fenceValue_);

	if (fence_->GetCompletedValue() < fenceValue_) {
		//指定したSignalにたどりついていないので、たどりつくまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		//イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}

	//解放したり、コマンド積めるようにしたり
	//----------------------------------------------//
	intermediateResource->Release();
	commandAllocator_->Reset();
	commandList_->Reset(commandAllocator_, nullptr);
	//----------------------------------------------//

	TextureData textureData{};
	textureData.resource = textureResource;
	textureData.metadata = metadata;
	textureData.srvHandleCPU = handleCPU;
	textureData.srvHandleGPU = handleGPU;
	textures_.push_back(textureData);
	return handleGPU;
}

D3D12_CPU_DESCRIPTOR_HANDLE ManagementTexture::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorheap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE ManagementTexture::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorheap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

D3D12_SHADER_RESOURCE_VIEW_DESC ManagementTexture::BuildSrvDesc(DirectX::TexMetadata metadata) {
	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dのテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	return srvDesc;
}

void ManagementTexture::Release() {
	for (TextureData& texture : textures_) {
		if (texture.resource) {
			texture.resource->Release();
			texture.resource = nullptr;
		}
	}

	textures_.clear();
	nextTextureIndex_ = 1;
}
