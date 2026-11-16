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
	SceneManager::GetInstance()->SetWidth(windowWidth);
	SceneManager::GetInstance()->SetHeight(windowHeight);
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

int EmpEngine::GetWhite1x1() {
	return EmpSystems::GetInstance()->GetWhite1x1();
}

void EmpEngine::PostDraw() {
	EmpSystems::GetInstance()->PostDraw();
}

// EmpEngine.cpp に追加
void EmpEngine::RebindRenderTarget() {
	EmpSystems::GetInstance()->RebindRenderTarget();
}

float EmpEngine::DeltaTime() {
	return EmpSystems::GetInstance()->DeltaTime();
}

std::mt19937& EmpEngine::GetRandom() {
	return EmpSystems::GetInstance()->GetRandom();
}