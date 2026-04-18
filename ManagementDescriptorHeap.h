#pragma once
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <wrl.h>

class ManagementDescriptorHeap {
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	ID3D12Resource* swapChainResources[2] = { nullptr };
public:
	void Initialize(ID3D12Device* device);
	void PullTheSwapChain(IDXGISwapChain4* swapChain);
	void GenerateRTV(ID3D12Device* device);
};