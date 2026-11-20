#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include<cassert>
#include<array>

class ManagementSwapChain {
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,2> swapChainResources_= { nullptr };

	void PullTheSwapChain();

public:
	void Initialize(int windowWidth,int windowHeight, ID3D12CommandQueue* comanndQueue,HWND hwnd, IDXGIFactory7* dxgiFactory);
	void Release();
	void ReleaseSwapChainBuffers();
	IDXGISwapChain4* GetSwapChain() const{ return swapChain_.Get(); }
	Microsoft::WRL::ComPtr<IDXGISwapChain4>GetSwapChainComPtr() { return swapChain_; }
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc_; }
	void Resize(UINT width, UINT height);
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> GetSwapChainResources() { return swapChainResources_; }
	void RecreateSwapChainBuffers(ID3D12Device* device, D3D12_RENDER_TARGET_VIEW_DESC rtvDesc, std::array<D3D12_CPU_DESCRIPTOR_HANDLE, 2>rtvHandles);
};