#include"EmpEngine.h"
#include"ClashHandler.h"

void EmpEngine::Initialize(int kWindowWidth, int kWindowHeight) {
	GetInstance().InitializeImpl(kWindowWidth, kWindowHeight);
}

EmpEngine& EmpEngine::GetInstance() {
	static EmpEngine instance;
	return instance;
}

void EmpEngine::InitializeImpl(int kWindowWidth,int kWindowHeight) {
	//誰も捕捉しなかった場合に(Unhandled)、捕捉する関数を登録
	//main関数が始まってすぐに登録すると良い
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
	managementDescriptHeap_.Initialize(managementDevice_.GetDevice(),managementSwapChain_.GetSwapChain());
	managementDXC_.Initialize(managementDevice_.GetDevice());
	managementViewPort_.Initialize(managementDevice_.GetDevice(), kWindowWidth, kWindowHeight);
	camera.Initialize();
	
}

int EmpEngine::ProcessMessage() {
	return GetInstance().ProcessMessageImpl();
}

int EmpEngine::ProcessMessageImpl() {
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

void EmpEngine::Finalize() {
	GetInstance().FinalizeImpl();
}

void EmpEngine::FinalizeImpl() {
	Release();

#ifdef _DEBUG
	managementDebug_.DebugReportLiveObject();
#endif
}

void EmpEngine::SetWindowSize(unsigned int index,int windowWidth,int windowHeight) {
	GetInstance().SetWindowSizeImpl(index, windowWidth, windowHeight);
}

void EmpEngine::SetWindowSizeImpl(unsigned int index,int windowWidth,int windowHeight) {
	managementWindow_.SetWindowSize(index, windowWidth, windowHeight);
}

void EmpEngine::BeginImpl() {
	managementCommand_.LoadCommand(managementSwapChain_.GetSwapChain(), managementDescriptHeap_.GetRtvHandles(),managementDescriptHeap_.GetSwapChainResources());
}

void EmpEngine::Begin() {
	GetInstance().BeginImpl();
}

void EmpEngine::Release() {
	managementCommand_.FenceRelease();

	managementDescriptHeap_.Release();
	managementSwapChain_.Release();
	managementCommand_.CommandRelease();
	managementDevice_.Release();
	managementDXGIFactory_.Release();
	managementDebug_.Release();
	managementWindow_.Release();
}

void EmpEngine::DrawTriangleImpl() {
	managementCommand_.DrawCall(
		managementViewPort_.GetViewPort(),
		managementViewPort_.GetScissorRect(),
		managementViewPort_.GetVertexBufferView(), 
		managementDXC_.GetGraphicPipeLineState(), 
		managementDXC_.GetRootSignature(),
		managementViewPort_.GetMaterialResource(),
		managementViewPort_.GetWvpResource());
}

void EmpEngine::DrawTriangle() {
	GetInstance().DrawTriangleImpl();
}

void EmpEngine::EndImpl() {
	managementCommand_.KickCommand(managementSwapChain_.GetSwapChain());
}

void EmpEngine::End() {
	GetInstance().EndImpl();
}

void EmpEngine::UpdateImpl(int kWindowWidth, int kWindowHeight) {
	camera.Update(managementViewPort_.GetTransform(), kWindowWidth, kWindowHeight);
	managementViewPort_.Update(camera);
}

void EmpEngine::Update(int kWindowWidth, int kWindowHeight) {
	GetInstance().UpdateImpl(kWindowWidth,kWindowHeight);
}