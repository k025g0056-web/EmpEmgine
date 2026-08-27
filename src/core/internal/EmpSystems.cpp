#include"EmpSystems.h"
#include"ClashHandler/CrashHandler.h"
#include"Log/ManagementLog.h"
#include"Scene/wrapper/SceneSystem.h"
#include<cassert>
#include"Interaction/Input/Input.h"

EmpSystems* EmpSystems::GetInstance() {
	static EmpSystems instance;
	return &instance;
}

void EmpSystems::Initialize(int kWindowWidth, int kWindowHeight) {
	//誰も捕捉しなかった場合に(Unhandled)、捕捉する関数を登録
	//main関数が始まってすぐに登録すると良い
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr) || hr == RPC_E_CHANGED_MODE);
	SetUnhandledExceptionFilter(CrashHandler::ExportDump);
	managementWindow_.Initialize(kWindowWidth, kWindowHeight);
	//debug initializeの場所ん
	managementDebug_.EnableDebugLayerWrapping();
	ManagementLog::Initialize();
	managementDXGIFactory_.Initialize();
	managementDXGIFactory_.DecideAdapter();
	Input::GetInstance()->Initialize(managementWindow_.GetHwnd(0));
	managementDevice_.CreateDevice(managementDXGIFactory_.GetUseAdapter());
	managementDebug_.ErrorDetection(managementDevice_.GetDevice());
	managementCommand_.Initialize(managementDevice_.GetDevice());
	managementSwapChain_.Initialize(kWindowWidth, kWindowHeight, managementCommand_.GetCommandQueue(), managementWindow_.GetHwnd(0), managementDXGIFactory_.GetDXGIFactory());
	managementViewPort_.Initialize(managementDevice_.GetDevice(), kWindowWidth, kWindowHeight);
	managementDescriptHeap_.Initialize(managementDevice_.GetDevice(), managementSwapChain_.GetSwapChain(),managementViewPort_.GetDepthStencilResource());
	managementDXC_.Initialize(managementDevice_.GetDevice());
#ifdef USE_IMGUI
	mymGui_.Initialize(managementWindow_.GetHwnd(0), managementDevice_.GetDevice(),
		managementSwapChain_.GetSwapChainDesc(), managementDescriptHeap_.GetRtvDesc(),
		managementDescriptHeap_.GetSrvDescriptorHeap());
#endif // USE_IMGUI
	renderTexture_.Initialize(
		managementDevice_.GetDevice(),
		kWindowWidth,
		kWindowHeight,
		managementDescriptHeap_.GetRenderTextureRtvHandle(),
		managementDescriptHeap_.GetRenderTextureSrvHandleCPU(),
		managementDescriptHeap_.GetRenderTextureSrvHandleGPU());
	loader_.Initialize(managementDevice_.GetDevice(),managementCommand_.GetCommandList(),managementCommand_.GetCommandQueue(),managementCommand_.GetCommandAllocator(),managementCommand_.GetFenceEvent(),managementCommand_.GetFenceValue(),managementCommand_.GetFence(),managementDescriptHeap_.GetSrvDescriptorHeap());
	white1x1 = loader_.Texture().Load("white1x1.png");

	managementLighting_.Initialize(managementDevice_.GetDevice());
	drawManager_.Initialize(managementDevice_.GetDevice());
	drawManager_.SetPostDrawFunc([this]() {PostDraw(); });
}

int EmpSystems::ProcessMessage() {
	MSG msg{};
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // ★if→while
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) {
			return 1;
		}
	}
	return 0;
}

void EmpSystems::Finalize() {
	Release();
	CoUninitialize();
#ifdef _DEBUG
	managementDebug_.DebugReportLiveObject();
#endif
}

void EmpSystems::SetWindowSize(unsigned int index, int windowWidth, int windowHeight) {
	managementWindow_.SetWindowSize(index, windowWidth, windowHeight);
}

void EmpSystems::Begin() {
	Input::GetInstance()->InputAllUpdate();
	drawManager_.ResetUsedCount();
	managementCommand_.LoadCommand(
		managementSwapChain_.GetSwapChain(), 
		managementDescriptHeap_.GetRtvHandles(),
		managementDescriptHeap_.GetSwapChainResources(),
		managementDescriptHeap_.GetDsvDescriptorHeap());
#ifdef USE_IMGUI
	mymGui_.Begin();
	mymGui_.MakeDockSpace();
	renderTexture_.Begin(managementCommand_.GetCommandList(), managementDescriptHeap_.GetDsvHandle());
#endif // USE_IMGUI
	drawManager_.SetCommandList(managementCommand_.GetCommandList());
}

void EmpSystems::End() {
#ifdef USE_IMGUI
	
	renderTexture_.End(managementCommand_.GetCommandList());

	UINT backBufferIndex = managementSwapChain_.GetSwapChain()->GetCurrentBackBufferIndex();
	managementCommand_.SetRenderTarget(
		managementDescriptHeap_.GetRtvHandles(backBufferIndex),
		managementDescriptHeap_.GetDsvHandle());
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	
	ImGui::Begin("Scene");

	ImVec2 sceneSize = ImGui::GetContentRegionAvail();
	if (sceneSize.x > 0.0f && sceneSize.y > 0.0f) {
		ImGui::Image(
			(ImTextureID)renderTexture_.GetSRV().ptr,
			sceneSize);
	}

	ImGui::End();
	ImGui::PopStyleVar();
	mymGui_.End(managementDescriptHeap_.GetSrvDescriptorHeap(), managementCommand_.GetCommandList());
#endif // USE_IMGUI

	managementCommand_.KickCommand(managementSwapChain_.GetSwapChain());
}

void EmpSystems::Release() {
	managementCommand_.WaitForGPU();
	mymGui_.Release();
	drawManager_.Release();
	managementLighting_.Release();
	loader_.Release();
	managementViewPort_.Release();
	managementDXC_.Release();
	managementDescriptHeap_.Release();
	managementSwapChain_.Release();
	managementCommand_.FenceRelease();
	managementCommand_.CommandRelease();
	managementDevice_.Release();
	managementDXGIFactory_.Release();
	managementDebug_.Release();
	managementWindow_.Release();
}


void EmpSystems::PostDraw() {
	managementCommand_.PostDraw(managementViewPort_.GetViewPort(),
		managementViewPort_.GetScissorRect(),
		managementDXC_.GetGraphicPipeLineState(),
		managementDXC_.GetRootSignature(), managementDescriptHeap_.GetSrvDescriptorHeap());
	managementLighting_.DrawCall(managementCommand_.GetCommandList());
}

void EmpSystems::LightGUI() {
	managementLighting_.GUI();
}

void EmpSystems::SetWindowColor(Vector4 color) {
	managementCommand_.SetClearColor(color);
}

void EmpSystems::DrawLight() {   
	drawManager_.Draw<Sphere>(
		Transform3d{ {0.05f,0.05f,0.05f},{0.0f,0.0f,0.0f},managementLighting_.GetDirection() },
		Vector4{ 1.0f,1.0f,1.0f,1.0f }, white1x1);
}

void EmpSystems::GeneratePSO() {
	managementDXC_.GeneratePSO();
}

void EmpSystems::SetBlendMode(BlendMode blendMode) {
	managementDXC_.SetBlendMode(blendMode);
}

void EmpSystems::SetRasterizer(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode) {
	managementDXC_.SetRasterizer(cullMode, fillMode);
}

void EmpSystems::SetDepthStencil(bool depthEnable, D3D12_DEPTH_WRITE_MASK DepthWriteMask, D3D12_COMPARISON_FUNC comparisonFunc) {
	managementDXC_.SetDepthStencil(depthEnable, DepthWriteMask, comparisonFunc);
}

void EmpSystems::SetPosition(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset) {
	managementDXC_.SetPosition(name, index, format, offset);
}

void EmpSystems::SetTexCoord(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset) {
	managementDXC_.SetTexCoord(name, index, format, offset);
}

void EmpSystems::SetNormal(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset) {
	managementDXC_.SetNormal(name, index, format, offset);
}

void EmpSystems::SetVertexShader(const std::wstring& filePath) {
	managementDXC_.SetVertexShader(filePath);
}

void EmpSystems::SetPixelShader(const std::wstring& filePath) {
	managementDXC_.SetPixelShader(filePath);
}

void EmpSystems::RebindRenderTarget() {
#ifdef USE_IMGUI
	// ImGui有効時はrenderTexture_へ描画してるので、そのRTVを使ってDSVごと張り直す
	managementCommand_.SetRenderTarget(
		managementDescriptHeap_.GetRenderTextureRtvHandle(),
		managementDescriptHeap_.GetDsvHandle());
#else
	// ImGui無効時はバックバッファへ直接描画してるので、現在のバックバッファを使う
	UINT backBufferIndex = managementSwapChain_.GetSwapChain()->GetCurrentBackBufferIndex();
	managementCommand_.SetRenderTarget(
		managementDescriptHeap_.GetRtvHandles(backBufferIndex),
		managementDescriptHeap_.GetDsvHandle());
#endif
}