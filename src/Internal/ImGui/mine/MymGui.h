#pragma once
#include<Windows.h>
#include<vector>
#ifdef USE_IMGUI
#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
#endif // USE_IMGUI
#include<d3d12.h>
#include<dxgi1_6.h>

class MymGui {

	void NewFrame();
	void Render();
	void SettingHeap(ID3D12DescriptorHeap* srvDescriptorHeap, ID3D12GraphicsCommandList* commnadList);
public:
	void Initialize(HWND hWnd,ID3D12Device* device,
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc, D3D12_RENDER_TARGET_VIEW_DESC rtvDesc,
		ID3D12DescriptorHeap* srvDescriptorHeap);
	void Begin();
	void DemoShowWindow();
	void MakeDockSpace();
	void End(ID3D12DescriptorHeap* srvDescriptorHeap,ID3D12GraphicsCommandList* commnadList);
	void Release();
};
