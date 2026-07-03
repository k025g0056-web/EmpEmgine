#include"EmpEngine.h"

EmpSystems empSystems;
SceneManager sceneManager_;

void EmpEngine::Initialize(int kWindowWidth, int kWindowHeight,SceneName scene) {
	empSystems.Initialize(kWindowWidth, kWindowHeight);
	sceneManager_.Initialize(kWindowWidth, kWindowHeight, scene);
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

void EmpEngine::End() {
	empSystems.End();
}

void EmpEngine::DrawTriangle(Vector3 v0, Vector3 v1,
	Vector3 v2, Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle) {
	empSystems.DrawTriangle(v0, v1, v2, color,GraphHandle);
}

void EmpEngine::DrawTriangle(Transform3d transform, Vector3 v0,
	Vector3 v1, Vector3 v2, Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle) {
	empSystems.DrawTriangleTrans(transform, v0, v1, v2, color,GraphHandle);
}


D3D12_GPU_DESCRIPTOR_HANDLE EmpEngine::LoadTexture(const std::string& str) {
	return empSystems.LoadTexture(str);
}

void EmpEngine::DrawSprite(const Transform3d& transform, const Vector2& v0,
	const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector4& color,
	D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle, const Transform3d& uvTransform) {
	empSystems.DrawSprite(transform, v0, v1, v2, v3, color, GraphHandle,uvTransform);
}

void EmpEngine::DrawTextureSphere(const Transform3d& transform, const Vector4& color
	, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle) {
	empSystems.DrawTextureSphere(transform, color, GraphHandle);
}

bool EmpEngine::EndManagement() {
	return sceneManager_.EndManagement();
}

void EmpEngine::Process() {
	sceneManager_.Process();
}

void EmpEngine::LightGUI() {
	empSystems.LightGUI();
}

void EmpEngine::DrawTriangleColor(const Vector3& v0, const Vector3& v1,
	const Vector3& v2, const Vector4 color) {
	empSystems.DrawTriangleColor(v0, v1, v2, color);
}

void EmpEngine::DrawTriangleColor(const Transform3d& transform, const Vector3& v0, const Vector3& v1,
	const Vector3& v2, const Vector4 color) {
	empSystems.DrawTriangleColor(transform, v0, v1, v2, color);
}

void EmpEngine::SetWindowColor(Vector4 color) {
	empSystems.SetWindowColor(color);
}

ModelData EmpEngine::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	return empSystems.LoadObjFile(directoryPath, filename);
}

void EmpEngine::DrawPreModel(const Transform3d& transform, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle, const ModelData& modelData_) {
	empSystems.DrawPreModel(transform, GraphHandle, modelData_);
}

SoundData EmpEngine::SoundLoadWave(const char* filename) {
	return empSystems.SoundLoadWave(filename);
}

void EmpEngine::PlayAudio(const SoundData& soundData) {
	empSystems.PlayAudio(soundData);
}

void EmpEngine::UnLoadAudio(SoundData* soundData) {
	empSystems.UnLoadAudio(soundData);
}

void EmpEngine::DrawLight() {
	empSystems.DrawLight();
}

ID3D12Device* EmpEngine::GetDevice() { return empSystems.GetDevice(); }

void EmpEngine::DrawCompressModel(
	const Transform3d& transform,
	D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle,
	Model& model) {
	empSystems.DrawCompressModel(transform, GraphHandle, model);
}

void EmpEngine::DrawSphere(const Transform3d& transform, const Vector4& color) {
	empSystems.DrawSphere(transform, color);
}