#include"ManagementDescriptorHeap.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<cassert>

void ManagementDescriptorHeap::Initialize(ID3D12Device* device, IDXGISwapChain4* swapChain) {
	GenerateRtvDescriptorHeap(device);
	PullTheSwapChain(swapChain);
	GenerateRTV(device);
	GenerateSrvDescriptorHeap(device);
}

void ManagementDescriptorHeap::GenerateRtvDescriptorHeap(ID3D12Device* device) {
	rtvDescriptorHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
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

void ManagementDescriptorHeap::Release() {
	rtvDescriptorHeap->Release();
	swapChainResources_[0]->Release();
	swapChainResources_[0] = nullptr;
	swapChainResources_[1]->Release();
	swapChainResources_[1] = nullptr;
}

ID3D12DescriptorHeap* ManagementDescriptorHeap::CreateDescriptorHeap(ID3D12Device* device, 
	D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible) {
	ID3D12DescriptorHeap* descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}

void ManagementDescriptorHeap::GenerateSrvDescriptorHeap(ID3D12Device* device) {
	srvDescriptorHeap_ = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
}