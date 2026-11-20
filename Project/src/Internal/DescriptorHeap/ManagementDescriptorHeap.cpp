#include"ManagementDescriptorHeap.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<cassert>
#include <stdexcept>

void ManagementDescriptorHeap::Initialize(ID3D12Device* device, IDXGISwapChain4* swapChain, ID3D12Resource* depthStencilResource, std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResorces) {
	GenerateRtvDescriptorHeap(device);
	GenerateRTV(device,swapChainResorces);
	GenerateSrvDescriptorHeap(device);
	GenerateDsvDescriptorHeap(device);
	GenerateDsvDesc(device, depthStencilResource);
}

void ManagementDescriptorHeap::GenerateRtvDescriptorHeap(ID3D12Device* device) {
	rtvDescriptorHeap_ = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 3, false);
}

void ManagementDescriptorHeap::GenerateRTV(ID3D12Device* device, std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources) {
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//出力結果をSRGBに書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2dテクスチャとして書き込む
	//ディスクリプタを取得する
	rtvStartHandle_ = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	//まず一つ目を作る。一つ目は最初のところに作る。作る場所を指定してあげる必要がある
	rtvHandles_[0] = rtvStartHandle_;
	device->CreateRenderTargetView(swapChainResources[0].Get(), &rtvDesc_, rtvHandles_[0]);
	//二つ目のディスクリプタハンドルを得る（自力で）
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//二つ目を作る
	device->CreateRenderTargetView(swapChainResources[1].Get(), &rtvDesc_, rtvHandles_[1]);
	renderTextureRtvHandle_.ptr = rtvHandles_[1].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
}

void ManagementDescriptorHeap::Release() {
	if (rtvDescriptorHeap_) {
		rtvDescriptorHeap_.Reset();
		rtvDescriptorHeap_ = nullptr;
	}
	
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
	srvDescriptorHeap_ = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, heap, true);
	srvDescriptorSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void ManagementDescriptorHeap::GenerateDsvDescriptorHeap(ID3D12Device* device) {
	dsvDescriptorHeap_ = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
}

void ManagementDescriptorHeap::GenerateDsvDesc(ID3D12Device*device,ID3D12Resource* depthStencilResource) {
	dsvDesc_.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//Format。基本的にはResourceに合わせる
	dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;//2dTexture
	//DSVHEAPの先頭にDSVを作る
	device->CreateDepthStencilView(depthStencilResource, &dsvDesc_, dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}


D3D12_CPU_DESCRIPTOR_HANDLE ManagementDescriptorHeap::GetRenderTextureSrvHandleCPU() const {
	D3D12_CPU_DESCRIPTOR_HANDLE handle = srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += srvDescriptorSize_ * nowHeap_;
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE ManagementDescriptorHeap::GetRenderTextureSrvHandleGPU() const {
	D3D12_GPU_DESCRIPTOR_HANDLE handle = srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += srvDescriptorSize_ * nowHeap_;
	return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE ManagementDescriptorHeap::GetDsvHandle() const {
	return dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
}

void ManagementDescriptorHeap::UpdateDsv(ID3D12Device* device,ID3D12Resource* depthStencilResource) {
	GenerateDsvDesc(device, depthStencilResource);
}

SrvSlot ManagementDescriptorHeap::AllocateSrv() {
	if (nowHeap_>=heap) {
		//後々増やす用の隙間
		throw std::runtime_error("SRV full");
	}

	UINT index = nowHeap_++;

	auto cpu = srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	auto gpu = srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();

	cpu.ptr += srvDescriptorSize_ * index;
	gpu.ptr += srvDescriptorSize_ * index;

	return { index,cpu,gpu };
}

D3D12_CPU_DESCRIPTOR_HANDLE ManagementDescriptorHeap::GetCPUPtr(UINT index) {
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHan = srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	cpuHan.ptr += srvDescriptorSize_ * index;
	return cpuHan;
}

D3D12_GPU_DESCRIPTOR_HANDLE ManagementDescriptorHeap::GetGPUPtr(UINT index) {
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHan = srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	gpuHan.ptr += srvDescriptorSize_ * index;
	return gpuHan;
}

void ManagementDescriptorHeap::RecreateRTV(ID3D12Device* device, std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources) {
	device->CreateRenderTargetView(
		swapChainResources[0].Get(), &rtvDesc_, rtvHandles_[0]);

	device->CreateRenderTargetView(
		swapChainResources[1].Get(), &rtvDesc_, rtvHandles_[1]);
}