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

void EmpEngine::DrawTriangleViewPort() {
	empSystems.DrawTriangleViewport();
}

void EmpEngine::End() {
	empSystems.End();
}

void EmpEngine::Update() {
	empSystems.Update();
}

void EmpEngine::DrawTriangle(Vector3 v0, Vector3 v1, Vector3 v2, Vector4 color) {
	empSystems.DrawTriangle(v0, v1, v2, color);
}

void EmpEngine::DrawTriangle(Transform3d transform, Vector3 v0, Vector3 v1, Vector3 v2, Vector4 color) {
	empSystems.DrawTriangleTrans(transform, v0, v1, v2, color);
}

void EmpEngine::DrawSpriteHomework() {
	empSystems.DrawSpriteHomework();
}