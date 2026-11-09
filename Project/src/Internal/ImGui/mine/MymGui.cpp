#include"MymGui.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<cassert>

void MymGui::Initialize(HWND hWnd, ID3D12Device* device,
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc, D3D12_RENDER_TARGET_VIEW_DESC rtvDesc,
	ID3D12DescriptorHeap* srvDescriptorHeap) {
#ifdef USE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX12_Init(device,
		swapChainDesc.BufferCount,
		rtvDesc.Format,
		srvDescriptorHeap,
		srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.Fonts->AddFontFromFileTTF(
		"C:/Windows/Fonts/msgothic.ttc",
		18.0f,
		nullptr,
		io.Fonts->GetGlyphRangesJapanese());



	//こいつは最後♪
	io.Fonts->Build();
#endif // USE_IMGUI
}

void MymGui::NewFrame() {
#ifdef USE_IMGUI
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif // USE_IMGUI
}

void MymGui::DemoShowWindow() {
#ifdef USE_IMGUI

	//開発用の処理ここを固有に切り替えるも良し
	ImGui::ShowDemoWindow();

#endif // USE_IMGUI
}

void MymGui::Render() {
#ifdef USE_IMGUI
	ImGui::Render();
#endif // USE_IMGUI
}

void MymGui::Begin() {
	NewFrame();
}

void MymGui::End(ID3D12DescriptorHeap* srvDescriptorHeap,ID3D12GraphicsCommandList* commnadList) {
#ifdef USE_IMGUI
	Render();
	SettingHeap(srvDescriptorHeap, commnadList);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commnadList);
#endif // USE_IMGUI
}

void MymGui::SettingHeap(ID3D12DescriptorHeap* srvDescriptorHeap, 
	ID3D12GraphicsCommandList* commnadList) {
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap };
	commnadList->SetDescriptorHeaps(1, descriptorHeaps);
}

void MymGui::Release() {
#ifdef USE_IMGUI
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // USE_IMGUI
}


void MymGui::MakeDockSpace() {
#ifdef USE_IMGUI
	ImGui::DockSpaceOverViewport();
#endif // USE_IMGUI

}