#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
class ManagementDescriptorHeap {
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_{};


	//RTVを二つ作るのでディスクリプタを二つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	D3D12_CPU_DESCRIPTOR_HANDLE renderTextureRtvHandle_{};
	UINT srvDescriptorSize_ = 0;

	ID3D12Resource* swapChainResources_[2] = { nullptr };

	void GenerateRtvDescriptorHeap(ID3D12Device* device);
	void GenerateSrvDescriptorHeap(ID3D12Device* device);
	void GenerateDsvDescriptorHeap(ID3D12Device* device);
	void PullTheSwapChain(IDXGISwapChain4* swapChain);
	void GenerateRTV(ID3D12Device* device);
	void GenerateDsvDesc(ID3D12Device* device, ID3D12Resource* depthStencilResource);
public:
	void Initialize(ID3D12Device* device, IDXGISwapChain4* swapChain, ID3D12Resource* depthStencilResource);

	static ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device,
		D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	ID3D12DescriptorHeap* GetRtvDescriptorHeap()const { return rtvDescriptorHeap_.Get(); }
	D3D12_DESCRIPTOR_HEAP_DESC GetRtvDescriptorHeapDesc() const { return rtvDescriptorHeapDesc_; }
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc()const { return rtvDesc_; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvStartHandle()const { return rtvStartHandle_; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandles(int i)const { if (i > 1 || 0 > i) { return {}; } return rtvHandles_[i]; }
	D3D12_CPU_DESCRIPTOR_HANDLE* GetRtvHandles() { return rtvHandles_; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTextureRtvHandle() const { return renderTextureRtvHandle_; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTextureSrvHandleCPU() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetRenderTextureSrvHandleGPU() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() const;
	ID3D12Resource* GetSwapChainResources(int i)const { if (i > 1 || 0 > i) { return {}; }return swapChainResources_[i]; }
	ID3D12Resource** GetSwapChainResources() { return swapChainResources_; }
	ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return srvDescriptorHeap_.Get(); }
	ID3D12DescriptorHeap* GetDsvDescriptorHeap() { return dsvDescriptorHeap_.Get(); }
	void Release();
};