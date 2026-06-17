#include"EmpSysetms.h"
#include"ClashHandler.h"
#include"ManagementLog.h"
#include"SceneSystem.h"
#include<cassert>

static int idx = 0;
static int douInd = 0;
static int sprInd = 0;
static int sphInd = 0;
static int modInd = 0;
void EmpSystems::Initialize(int kWindowWidth, int kWindowHeight) {
	//誰も捕捉しなかった場合に(Unhandled)、捕捉する関数を登録
	//main関数が始まってすぐに登録すると良い
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr) || hr == RPC_E_CHANGED_MODE);
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
#ifdef USE_IMGUI
	mymGui_.Initialize(managementWindow_.GetHwnd(0), managementDevice_.GetDevice(),
		managementSwapChain_.GetSwapChainDesc(), managementDescriptHeap_.GetRtvDesc(),
		managementDescriptHeap_.GetSrvDescriptorHeap());
#endif // USE_IMGUI
	white1x1 = LoadTexture("resources/white1x1.png");

	managementLighting_.Initialize(managementDevice_.GetDevice());
	managementAudio_.Initialize();
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


void EmpSystems::DrawDoubleTriangle(const Transform3d& transform, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle) {
	douInd = drawManager_.AddDoubleTriangle(managementDevice_.GetDevice());
	PostDraw();
	drawManager_.GetDoubleTriangle(douInd).
		DrawDoubleTriangle(transform, managementCommand_.GetCommandList(), GraphHandle,*SceneSystem::GetCamera());
}

void EmpSystems::End() {
#ifdef USE_IMGUI
	mymGui_.End(managementDescriptHeap_.GetSrvDescriptorHeap(), managementCommand_.GetCommandList());
#endif // USE_IMGUI

	managementCommand_.KickCommand(managementSwapChain_.GetSwapChain());
}

void EmpSystems::Release() {
	mymGui_.Release();
	drawManager_.Release();
	managementLighting_.Release();
	managementTexture_.Release();
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

D3D12_GPU_DESCRIPTOR_HANDLE EmpSystems::LoadTexture(const std::string& filepath) {
	return managementTexture_.LoadTexture(managementDevice_.GetDevice(), filepath,managementCommand_.GetCommandList()
	,managementCommand_.GetCommandQueue(),managementCommand_.GetCommandAllocator(),managementCommand_.GetFenceEvent()
	,managementCommand_.GetFenceValue(),managementCommand_.GetFence(),managementDescriptHeap_.GetSrvDescriptorHeap());
}

void EmpSystems::PostDraw() {
	managementCommand_.PostDraw(managementViewPort_.GetViewPort(),
		managementViewPort_.GetScissorRect(),
		managementDXC_.GetGraphicPipeLineState(),
		managementDXC_.GetRootSignature(), managementDescriptHeap_.GetSrvDescriptorHeap());
	managementLighting_.DrawCall(managementCommand_.GetCommandList());
}

void EmpSystems::DrawTriangle(const Vector3& v0, const Vector3& v1, 
	const Vector3& v2, const Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle) {
	idx = drawManager_.AddTriangle(managementDevice_.GetDevice());
	PostDraw();
	drawManager_.GetTriangle(idx).DrawTriangle(v0, v1, v2, managementCommand_.GetCommandList(),
		GraphHandle, color);
}

void EmpSystems::DrawTriangleTrans(const Transform3d& transform, const Vector3& v0,
	const Vector3& v1, const Vector3& v2, const Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle) {
	idx = drawManager_.AddTriangle(managementDevice_.GetDevice());;
	PostDraw();
	drawManager_.GetTriangle(idx).DrawTriangle(transform,v0, v1, v2, managementCommand_.GetCommandList(), 
		GraphHandle, color,*SceneSystem::GetCamera());

}

void EmpSystems::DrawSprite(const Transform3d& transform, const Vector2& v0,
	const Vector2& v1, const Vector2& v2, const Vector2& v3,const Vector4& color,
	D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle, const Transform3d& uvTransform) {
	sprInd = drawManager_.AddSprite(managementDevice_.GetDevice());
	PostDraw();
	drawManager_.GetSprite(sprInd).DrawSprite(transform,v0,v1,v2,v3,managementCommand_.GetCommandList(),
		GraphHandle,color,*SceneSystem::GetCamera(),uvTransform);
}

void EmpSystems::DrawSphere(const Transform3d& transform, const Vector4& color
	, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle) {
	sphInd = drawManager_.AddSphere(managementDevice_.GetDevice());
	PostDraw();
	drawManager_.GetSphere(sphInd).DrawSphere(transform, color, 
		managementCommand_.GetCommandList(), GraphHandle,*SceneSystem::GetCamera());
}

void EmpSystems::DrawColorSphere(const Transform3d& transform, const Vector4& color) {
	DrawSphere(transform, color, white1x1);
}

void EmpSystems::DrawQuad(const Transform3d& transform, const Vector2& v0,
	const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector4& color) {
	DrawSprite(transform, v0, v1, v2, v3, color, white1x1,{ReturnAllOne(),{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}});
}

void EmpSystems::LightGUI() {
	managementLighting_.GUI();
}

void EmpSystems::DrawTriangleColor(const Vector3& v0, const Vector3& v1,
	const Vector3& v2, const Vector4 color) {
	DrawTriangle(v0, v1, v2, color, white1x1);
}

void EmpSystems::DrawTriangleColor(const Transform3d& transform, const Vector3& v0, const Vector3& v1,
	const Vector3& v2, const Vector4 color) {
	DrawTriangleTrans(transform,v0, v1, v2, color, white1x1);
}

void EmpSystems::SetWindowColor(Vector4 color) {
	managementCommand_.SetClearColor(color);
}

ModelData EmpSystems::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	return managementModel_.LoadObjFile(directoryPath, filename);
}

void EmpSystems::DrawPreModel(const Transform3d& transform, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle,const ModelData& modelData) {
	modInd = drawManager_.AddModel(managementDevice_.GetDevice(), modelData);
	PostDraw();
	drawManager_.GetModel(modInd).DrawModel(transform, managementCommand_.GetCommandList(), GraphHandle,
		{ 1.0f,1.0f,1.0f,1.0f }, *SceneSystem::GetCamera());

}

SoundData EmpSystems::SoundLoadWave(const char* filename) {
	return managementAudio_.SoundLoadWave(filename);
}

void EmpSystems::PlayAudio(const SoundData& soundData) {
	managementAudio_.SoundPlayWave(soundData);
}

void EmpSystems::UnLoadAudio(SoundData* soundData) {
	managementAudio_.SoundUnload(soundData);
}