#include"DX12Mechanics.h"
#include<cassert>

ID3D12Resource* DX12Mechanics::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes, WhichResource resour) {
	ID3D12Resource* resource = nullptr;
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う

	//バッファリソーステクスチャの場合また別の設定をする
	D3D12_RESOURCE_DESC desc{};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	//リソースのサイズ
	//--------------------------------------------------//
	switch (resour) {
	case WhichResource::Vertex:
	case WhichResource::Index:
		desc.Width = sizeInBytes;
		break;
	case WhichResource::Constant:
		desc.Width = (sizeInBytes + 255) & ~255;
		break;
	}
	//---------------------------------------------------//

	//１にする決まりのゾーン
	//--------------------------------------------------//
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	//--------------------------------------------------//

	//バッファはこれにする決まり
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource)
	);
	assert(SUCCEEDED(hr));

	return resource;
}