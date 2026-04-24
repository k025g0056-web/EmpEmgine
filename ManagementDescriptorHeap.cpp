#include"ManagementDescriptorHeap.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")


void ManagementDescriptorHeap::Initialize(ID3D12Device* device, IDXGISwapChain4* swapChain) {
	GenerateDescriptHeap(device);
	PullTheSwapChain(swapChain);
	GenerateRTV(device);
}

void ManagementDescriptorHeap::GenerateDescriptHeap(ID3D12Device* device) {
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー用
	rtvDescriptorHeapDesc.NumDescriptors = 2;//ダブルバッファ用に二つ。多くても別にかまわない
	HRESULT hr = device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));
	//ディスクリプタヒープが作れなかったので起動できない
	assert(SUCCEEDED(hr));
}

void ManagementDescriptorHeap::PullTheSwapChain(IDXGISwapChain4* swapChain) {
	HRESULT hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	//上手く取得出来なければ起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
	assert(SUCCEEDED(hr));
}

void ManagementDescriptorHeap::GenerateRTV(ID3D12Device* device) {
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//出力結果をSRGBに書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2dテクスチャとして書き込む
	//ディスクリプタを取得する
	rtvStartHandle_ = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//まず一つ目を作る。一つ目は最初のところに作る。作る場所を指定してあげる必要がある
	rtvHandles_[0] = rtvStartHandle_;
	device->CreateRenderTargetView(swapChainResources_[0], &rtvDesc, rtvHandles_[0]);
	//二つ目のディスクリプタハンドルを得る（自力で）
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//二つ目を作る
	device->CreateRenderTargetView(swapChainResources_[1], &rtvDesc, rtvHandles_[1]);
	
}