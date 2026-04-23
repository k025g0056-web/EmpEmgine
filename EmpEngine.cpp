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
	managementCommand_.Initialize(managementDevice_.GetDevice());
	managementSwapChain_.Initialize(kWindowWidth, kWindowHeight, managementCommand_.GetCommandQueue(), managementWindow_.GetHwnd(0), managementDXGIFactory_.GetDXGIFactory());
	managementDescriptHeap_.Initialize(managementDevice_.GetDevice(),managementSwapChain_.GetSwapChain());
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
	
}

void EmpEngine::SetWindowSize(unsigned int index,int windowWidth,int windowHeight) {
	GetInstance().SetWindowSizeImpl(index, windowWidth, windowHeight);
}

void EmpEngine::SetWindowSizeImpl(unsigned int index,int windowWidth,int windowHeight) {
	managementWindow_.SetWindowSize(index, windowWidth, windowHeight);
}

void EmpEngine::BeginImpl() {

	managementCommand_.LoadCommand(managementSwapChain_.GetSwapChain(), managementDescriptHeap_.GetRtvHandles());
	managementCommand_.KickCommand(managementSwapChain_.GetSwapChain());
}

void EmpEngine::Begin() {
	GetInstance().BeginImpl();
}