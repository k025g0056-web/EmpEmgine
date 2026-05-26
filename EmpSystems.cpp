#include"EmpSysetms.h"
#include"ClashHandler.h"
#include"ManagementLog.h"

static int idx = 0;
static int douInd = 0;
static int sprInd = 0;
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
	drawManager_.Release();
	managementCommand_.LoadCommand(
		managementSwapChain_.GetSwapChain(), 
		managementDescriptHeap_.GetRtvHandles(),
		managementDescriptHeap_.GetSwapChainResources(),
		managementDescriptHeap_.GetDsvDescriptorHeap());
#ifdef USE_IMGUI
	mymGui_.Begin();
#endif // USE_IMGUI
}

void EmpSystems::DrawTriangleViewport() {
	douInd = drawManager_.AddDoubleTriangle(managementDevice_.GetDevice());
	PostDraw();
	drawManager_.GetDoubleTriangle(douInd).
		DrawDoubleTriangle(transform, managementCommand_.GetCommandList(), SRV, camera_);
}

void EmpSystems::End() {
#ifdef USE_IMGUI
	mymGui_.End(managementDescriptHeap_.GetSrvDescriptorHeap(), managementCommand_.GetCommandList());
#endif // USE_IMGUI

	managementCommand_.KickCommand(managementSwapChain_.GetSwapChain());
}

void EmpSystems::Update() {
	Vector4 color = SprColor * 255.0f;

#ifdef USE_IMGUI
	//GUIエリア☆（ECCジュニアのリズムで）
	ImGui::SliderFloat4("Material", &color.x, 0.0f, 255.0f, "%3f", 0);
	ImGui::SliderFloat3("Transform", &transformSpr.translate.x, 0.0f, 1280.0f, "%3f", 0);
#endif // USE_IMGUI
	transform.rotate.y += 0.03f;
	SprColor = color / 255.0f;
	camera_.Update(windowWidth_, windowHeight_);
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
	drawManager_.Release();
}

ID3D12Resource* EmpSystems::LoadTexture(const std::string& filepath) {
	return managementTexture_.LoadTexture(managementDevice_.GetDevice(), filepath,managementCommand_.GetCommandList()
	,managementCommand_.GetCommandQueue(),managementCommand_.GetCommandAllocator(),managementCommand_.GetFenceEvent()
	,managementCommand_.GetFenceValue(),managementCommand_.GetFence());
}

void EmpSystems::PostDraw() {
	managementCommand_.PostDraw(managementViewPort_.GetViewPort(),
		managementViewPort_.GetScissorRect(),
		managementDXC_.GetGraphicPipeLineState(),
		managementDXC_.GetRootSignature(), managementDescriptHeap_.GetSrvDescriptorHeap());
}

void EmpSystems::DrawTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector4 color) {
	idx = drawManager_.AddTriangle(managementDevice_.GetDevice());
	PostDraw();
	drawManager_.GetTriangle(idx).DrawTriangle(v0, v1, v2, managementCommand_.GetCommandList(),
		SRV, color);
}

void EmpSystems::DrawTriangleTrans(const Transform3d& transform, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector4 color) {
	idx = drawManager_.AddTriangle(managementDevice_.GetDevice());;
	PostDraw();
	drawManager_.GetTriangle(idx).DrawTriangle(transform,v0, v1, v2, managementCommand_.GetCommandList(), 
		SRV, color,camera_);

}

void EmpSystems::DrawSprite(const Transform3d& transform, const Vector2& v0,
	const Vector2& v1, const Vector2& v2, const Vector2& v3,const Vector4& color) {
	sprInd = drawManager_.AddSprite(managementDevice_.GetDevice());
	PostDraw();
	drawManager_.GetSprite(sprInd).DrawSprite(transform,v0,v1,v2,v3,managementCommand_.GetCommandList(),
		SRV,color,camera_);
}

void EmpSystems::DrawSpriteHomework() {
	DrawSprite(transformSpr, { 0.0f,360.0f }, { 0.0f,0.0f }, { 640.0f,360.0f }, { 640.0f,0.0f },SprColor);
}