#include"MymGui.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<cassert>

void MymGui::Initialize(HWND hWnd, ID3D12Device* device,
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc, D3D12_RENDER_TARGET_VIEW_DESC rtvDesc,
	ID3D12DescriptorHeap* srvDescriptorHeap) {
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

	io.Fonts->AddFontFromFileTTF(
		"C:/Windows/Fonts/msgothic.ttc",
		18.0f,
		nullptr,
		io.Fonts->GetGlyphRangesJapanese());



	//こいつは最後♪
	io.Fonts->Build();
}

void MymGui::NewFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void MymGui::DemoShowWindow() {
	//開発用の処理ここを固有に切り替えるも良し
	ImGui::ShowDemoWindow();
}

void MymGui::Render() {
	ImGui::Render();
}

void MymGui::Begin() {
	NewFrame();
}

void MymGui::End(ID3D12DescriptorHeap* srvDescriptorHeap,ID3D12GraphicsCommandList* commnadList) {
	Render();
	SettingHeap(srvDescriptorHeap, commnadList);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commnadList);
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