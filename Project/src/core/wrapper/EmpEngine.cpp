#include"EmpEngine.h"

void EmpEngine::Initialize(int kWindowWidth, int kWindowHeight,SceneName scene) {
	EmpSystems::GetInstance()->Initialize(kWindowWidth, kWindowHeight);
	SceneManager::GetInstance()->Initialize(kWindowWidth, kWindowHeight, scene);
}

int EmpEngine::ProcessMessage() {
	return EmpSystems::GetInstance()->ProcessMessage();
}

void EmpEngine::Finalize() {
	EmpSystems::GetInstance()->Finalize();
}

void EmpEngine::SetWindowSize(unsigned int index,int windowWidth,int windowHeight) {
	EmpSystems::GetInstance()->SetWindowSize(index, windowWidth, windowHeight);
}

void EmpEngine::Begin() {
	EmpSystems::GetInstance()->Begin();
}

void EmpEngine::End() {
	EmpSystems::GetInstance()->End();
}

bool EmpEngine::EndManagement() {
	return SceneManager::GetInstance()->EndManagement();
}

void EmpEngine::Process() {
	SceneManager::GetInstance()->Update();
	SceneManager::GetInstance()->Draw();

}

void EmpEngine::LightGUI() {
	EmpSystems::GetInstance()->LightGUI();
}

void EmpEngine::SetWindowColor(Vector4 color) {
	EmpSystems::GetInstance()->SetWindowColor(color);
}

LoaderManager& EmpEngine::Resource() {
	return EmpSystems::GetInstance()->Resource();
}

DrawManager& EmpEngine::Draw() {
	return EmpSystems::GetInstance()->Draw();
}

void EmpEngine::DrawLight() {
	EmpSystems::GetInstance()->DrawLight();
}

ID3D12Device* EmpEngine::GetDevice() { return EmpSystems::GetInstance()->GetDevice(); }

void EmpEngine::GeneratePSO() {
	EmpSystems::GetInstance()->GeneratePSO();
}

void EmpEngine::SetBlendMode(BlendMode blendMode) {
	EmpSystems::GetInstance()->SetBlendMode(blendMode);
}

void EmpEngine::SetRasterizer(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode) {
	EmpSystems::GetInstance()->SetRasterizer(cullMode, fillMode);
}

void EmpEngine::SetDepthStencil(bool depthEnable, D3D12_DEPTH_WRITE_MASK DepthWriteMask, D3D12_COMPARISON_FUNC comparisonFunc) {
	EmpSystems::GetInstance()->SetDepthStencil(depthEnable, DepthWriteMask, comparisonFunc);
}

void EmpEngine::SetPosition(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset) {
	EmpSystems::GetInstance()->SetPosition(name, index, format, offset);
}

void EmpEngine::SetTexCoord(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset) {
	EmpSystems::GetInstance()->SetTexCoord(name, index, format, offset);
}

void EmpEngine::SetNormal(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset) {
	EmpSystems::GetInstance()->SetNormal(name, index, format, offset);
}

void EmpEngine::SetVertexShader(const std::wstring& filePath) {
	EmpSystems::GetInstance()->SetVertexShader(filePath);
}

void EmpEngine::SetPixelShader(const std::wstring& filePath) {
	EmpSystems::GetInstance()->SetPixelShader(filePath);
}

D3D12_GPU_DESCRIPTOR_HANDLE EmpEngine::GetWhite1x1() {
	return EmpSystems::GetInstance()->GetWhite1x1();
}

void EmpEngine::PostDraw() {
	EmpSystems::GetInstance()->PostDraw();
}

// EmpEngine.cpp に追加
void EmpEngine::RebindRenderTarget() {
	EmpSystems::GetInstance()->RebindRenderTarget();
}