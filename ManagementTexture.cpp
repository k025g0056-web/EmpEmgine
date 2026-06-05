#include"ManagementTexture.h"
#pragma comment(lib,"d3d12.lib")
#include"ManagementLog.h"
#include"DX12Mechanics.h"
#include"externals//DirectXTex/d3dx12.h"

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
	std::wstring filePathW = ManagementLog::ConvertString(filepath);

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

ID3D12Resource* ManagementTexture::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {
	//1.metadetaを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	BuildResourceDesc(resourceDesc, metadata);
	//2.利用するheapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	SettingHeap(heapProperties);
	//3.Resourceの精製
	ID3D12Resource* resource = nullptr;
	CreateCommittedResource(resource, resourceDesc, heapProperties, device);
	return resource;
}

void ManagementTexture::BuildResourceDesc(D3D12_RESOURCE_DESC& resourceDesc,const DirectX::TexMetadata& metadata) {
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

void ManagementTexture::CreateCommittedResource(ID3D12Resource*& resource, D3D12_RESOURCE_DESC& resourceDesc,
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
	ID3D12Resource* intermediateResource = DX12Mechanics::CreateBufferResource(device, intermediateSize);
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

D3D12_GPU_DESCRIPTOR_HANDLE ManagementTexture::LoadTexture(ID3D12Device* device, const std::string& filePath,
	ID3D12GraphicsCommandList* commandList,ID3D12CommandQueue* commandQueue,
	ID3D12CommandAllocator* commandAllocator, HANDLE fenceEvent, uint64_t fenceValue, ID3D12Fence*fence, ID3D12DescriptorHeap* srvDescriptorHeap) {
	DirectX::ScratchImage mipImages;
	DirectX::TexMetadata metadata;
	ID3D12Resource* textureResource = nullptr;
	//テクスチャを読んで転送する
	mipImages = LoadTextureFile(filePath);
	metadata = mipImages.GetMetadata();
	textureResource = CreateTextureResource(device, metadata);
	ID3D12Resource* intermediateResource=UploadTextureData(textureResource, mipImages,device,commandList);
	
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = BuildSrvDesc(metadata);
	const uint32_t descriptorSizeSRV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	const uint32_t descriptorSizeRTV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	const uint32_t descriptorSizeDSV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, nextTextureIndex_);
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, nextTextureIndex_);
	device->CreateShaderResourceView(textureResource, &srvDesc, handleCPU);
	nextTextureIndex_++;

	//リストを閉じる
	commandList->Close();

	ID3D12CommandList* commandLists[] = { commandList};
	commandQueue->ExecuteCommandLists(1, commandLists);

	//シグナルを送る
	commandQueue->Signal(fence, fenceValue);

	if (fence->GetCompletedValue() < fenceValue) {
		//指定したSignalにたどりついていないので、たどりつくまで待つようにイベントを設定する
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	//解放したり、コマンド積めるようにしたり
	//----------------------------------------------//
	intermediateResource->Release();
	commandAllocator->Reset();
	commandList->Reset(commandAllocator, nullptr);
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
