#include"EmpEngine.h"

EmpSystems empSystems;

void EmpEngine::Initialize(int kWindowWidth, int kWindowHeight) {
	empSystems.Initialize(kWindowWidth, kWindowHeight);
}

int EmpEngine::ProcessMessage() {
	return empSystems.ProcessMessage();
}

void EmpEngine::Finalize() {
	empSystems.Finalize();
}

void EmpEngine::SetWindowSize(unsigned int index,int windowWidth,int windowHeight) {
	empSystems.SetWindowSize(index, windowWidth, windowHeight);
}

void EmpEngine::Begin() {
	empSystems.Begin();
}

void EmpEngine::DrawTriangle() {
	empSystems.DrawTriangle();
}

void EmpEngine::End() {
	empSystems.End();
}

void EmpEngine::Update() {
	empSystems.Update();
}