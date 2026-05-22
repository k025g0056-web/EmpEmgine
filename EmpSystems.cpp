#include"EmpSysetms.h"
#include"ClashHandler.h"
#include"ManagementLog.h"

void EmpSystems::Initialize(int kWindowWidth, int kWindowHeight) {
	//誰も捕捉しなかった場合に(Unhandled)、捕捉する関数を登録
	//main関数が始まってすぐに登録すると良い
	CoInitializeEx(0, COINIT_MULTITHREADED);
	SetUnhandledExceptionFilter(ClashHandler::ExportDump);
	managementWindow_.Initialize(kWindowWidth, kWindowHeight);
	//debug initializeの場所ん
	managementDebug_.EnableDebugLayerWrapping();
	ManagementLog::Initialize();
	managementDXGIFactory_.Initialize();
	managementDXGIFactory_.DecideAdapter();
	managementDevice_.CreateDevice(managementDXGIFactory_.GetUseAdapter());
	managementDebug_.ErrorDetection(managementDevice_.GetDevice());
	managementCommand_.Initialize(managementDevice_.GetDevice());
	managementSwapChain_.Initialize(kWindowWidth, kWindowHeight, managementCommand_.GetCommandQueue(), managementWindow_.GetHwnd(0), managementDXGIFactory_.GetDXGIFactory());
	managementViewPort_.Initialize(managementDevice_.GetDevice(), kWindowWidth, kWindowHeight);
	managementDescriptHeap_.Initialize(managementDevice_.GetDevice(), managementSwapChain_.GetSwapChain(),managementViewPort_.GetDepthStencilResource());
	managementDXC_.Initialize(managementDevice_.GetDevice());
	camera_.Initialize();
	windowHeight_ = kWindowHeight;
	windowWidth_ = kWindowWidth;
#ifdef USE_IMGUI
	mymGui_.Initialize(managementWindow_.GetHwnd(0), managementDevice_.GetDevice(),
		managementSwapChain_.GetSwapChainDesc(), managementDescriptHeap_.GetRtvDesc(),
		managementDescriptHeap_.GetSrvDescriptorHeap());
#endif // USE_IMGUI
	LoadTexture("resources/uvChecker.png");
	SRV = managementTexture_.CreateSRV(managementDescriptHeap_.GetSrvDescriptorHeap(), managementDevice_.GetDevice());
}

int EmpSystems::ProcessMessage() {
	MSG msg{};
	//Windowにメッセージが来ていたら最優先で処理させる
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
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
	managementCommand_.LoadCommand(
		managementSwapChain_.GetSwapChain(), 
		managementDescriptHeap_.GetRtvHandles(),
		managementDescriptHeap_.GetSwapChainResources(),
		managementDescriptHeap_.GetDsvDescriptorHeap());
#ifdef USE_IMGUI
	mymGui_.Begin();
#endif // USE_IMGUI
}

void EmpSystems::DrawTriangle() {
	managementCommand_.DrawCall(
		managementViewPort_.GetViewPort(),
		managementViewPort_.GetScissorRect(),
		managementViewPort_.GetVertexBufferView(),
		managementDXC_.GetGraphicPipeLineState(),
		managementDXC_.GetRootSignature(),
		managementViewPort_.GetMaterialResource(),
		managementViewPort_.GetWvpResource(),
		managementDescriptHeap_.GetSrvDescriptorHeap(),
		SRV);
}

void EmpSystems::End() {
#ifdef USE_IMGUI
	mymGui_.End(managementDescriptHeap_.GetSrvDescriptorHeap(), managementCommand_.GetCommandList());
#endif // USE_IMGUI

	managementCommand_.KickCommand(managementSwapChain_.GetSwapChain());
}

void EmpSystems::Update() {
#ifdef USE_IMGUI
	//GUIエリア☆（ECCジュニアのリズムで）


#endif // USE_IMGUI


	camera_.Update(managementViewPort_.GetTransform(), windowWidth_, windowHeight_);
	managementViewPort_.Update(camera_);
}

void EmpSystems::Release() {
	managementCommand_.FenceRelease();
	managementViewPort_.Release();
	managementDXC_.Release();
	managementDescriptHeap_.Release();
	managementSwapChain_.Release();
	managementCommand_.CommandRelease();
	managementDevice_.Release();
	managementDXGIFactory_.Release();
	managementDebug_.Release();
	managementWindow_.Release();
	mymGui_.Release();
}

ID3D12Resource* EmpSystems::LoadTexture(const std::string& filepath) {
	return managementTexture_.LoadTexture(managementDevice_.GetDevice(), filepath,managementCommand_.GetCommandList()
	,managementCommand_.GetCommandQueue(),managementCommand_.GetCommandAllocator(),managementCommand_.GetFenceEvent()
	,managementCommand_.GetFenceValue(),managementCommand_.GetFence());
}