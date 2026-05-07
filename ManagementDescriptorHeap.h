#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
class ManagementDescriptorHeap {
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;
	//RTVを二つ作るのでディスクリプタを二つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];

	ID3D12Resource* swapChainResources_[2] = { nullptr };

	void GenerateDescriptHeap(ID3D12Device* device);
	void PullTheSwapChain(IDXGISwapChain4* swapChain);
	void GenerateRTV(ID3D12Device* device);
public:
	void Initialize(ID3D12Device* device, IDXGISwapChain4* swapChain);

	ID3D12DescriptorHeap* GetRtvDescriptorHeap()const { return rtvDescriptorHeap; }
	D3D12_DESCRIPTOR_HEAP_DESC GetRtvDescriptorHeapDesc() const { return rtvDescriptorHeapDesc; }
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc()const { return rtvDesc; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvStartHandle()const { return rtvStartHandle_; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandles(int i)const { if (i > 1 || 0 > i) { return {}; } return rtvHandles_[i]; }
	D3D12_CPU_DESCRIPTOR_HANDLE* GetRtvHandles() {return rtvHandles_; }
	ID3D12Resource* GetSwapChainResources(int i)const { if (i > 1 || 0 > i) { return {}; }return swapChainResources_[i]; }
	ID3D12Resource** GetSwapChainResources() { return swapChainResources_; }
};