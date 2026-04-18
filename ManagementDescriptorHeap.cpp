#include"ManagementDescriptorHeap.h"

void ManagementDescriptorHeap::Initialize(ID3D12Device* device) {
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー用
	rtvDescriptorHeapDesc.NumDescriptors = 2;//ダブルバッファ用に二つ。多くても別にかまわない
	HRESULT hr = device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));
	//ディスクリプタヒープが作れなかったので起動できない
	assert(SUCCEEDED(hr));
}

void ManagementDescriptorHeap::PullTheSwapChain(IDXGISwapChain4* swapChain) {
	HRESULT hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
	//上手く取得出来なければ起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	assert(SUCCEEDED(hr));
}

void ManagementDescriptorHeap::GenerateRTV(ID3D12Device* device) {

}