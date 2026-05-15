#include"ManagementTexture.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"d3d12.lib")
#include"ManagementLog.h"

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
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//プロセッサの近くに配置
}

void ManagementTexture::GenerateResource(ID3D12Resource*& resource, D3D12_RESOURCE_DESC& resourceDesc,
	D3D12_HEAP_PROPERTIES& heapProperties, ID3D12Device* device) {
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//HEAPの特殊な設定。特になし
		&resourceDesc,//Resourceの設定
		D3D12_RESOURCE_STATE_GENERIC_READ,//初回のResourceState。Textureは基本的に読むだけ
		nullptr,//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));
}

void ManagementTexture::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {
	//Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels;++mipLevel) {
		//MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//Textureに転送
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,//全領域にコピー
			img->pixels,//元データアドレス
			UINT(img->rowPitch),//1ラインサイズ
			UINT(img->slicePitch)//1マイサイズ
		);
		assert(SUCCEEDED(hr));

	}
}

ID3D12Resource* ManagementTexture::LoadTexture(ID3D12Device* device, const std::string& filePath) {
	//テクスチャを読んで転送する
	DirectX::ScratchImage mipImages = LoadTextureFile(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(device, metadata);
	UploadTextureData(textureResource, mipImages);
	return textureResource;
}
