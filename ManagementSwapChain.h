#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include<cassert>

class ManagementSwapChain {
	IDXGISwapChain4* swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

public:
	void Initialize(int windowWidth,int windowHeight, ID3D12CommandQueue* comanndQueue,HWND hwnd, IDXGIFactory7* dxgiFactory);
	void Release();

	IDXGISwapChain4* GetSwapChain() const{ return swapChain_; }
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc_; }

};