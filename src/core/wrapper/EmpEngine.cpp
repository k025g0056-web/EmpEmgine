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

void EmpEngine::DrawTriangle(Vector3 v0, Vector3 v1,
	Vector3 v2, Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle) {
	EmpSystems::GetInstance()->DrawTriangle(v0, v1, v2, color,GraphHandle);
}

void EmpEngine::DrawTriangle(Transform3d transform, Vector3 v0,
	Vector3 v1, Vector3 v2, Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle) {
	EmpSystems::GetInstance()->DrawTriangleTrans(transform, v0, v1, v2, color,GraphHandle);
}

void EmpEngine::DrawSprite(const Transform3d& transform, const Vector2& v0,
	const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector4& color,
	D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle, const Transform3d& uvTransform) {
	EmpSystems::GetInstance()->DrawSprite(transform, v0, v1, v2, v3, color, GraphHandle,uvTransform);
}

void EmpEngine::DrawTextureSphere(const Transform3d& transform, const Vector4& color
	, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle) {
	EmpSystems::GetInstance()->DrawTextureSphere(transform, color, GraphHandle);
}

bool EmpEngine::EndManagement() {
	return SceneManager::GetInstance()->EndManagement();
}

void EmpEngine::Process() {
	SceneManager::GetInstance()->Process();
}

void EmpEngine::LightGUI() {
	EmpSystems::GetInstance()->LightGUI();
}

void EmpEngine::DrawTriangleColor(const Vector3& v0, const Vector3& v1,
	const Vector3& v2, const Vector4 color) {
	EmpSystems::GetInstance()->DrawTriangleColor(v0, v1, v2, color);
}

void EmpEngine::DrawTriangleColor(const Transform3d& transform, const Vector3& v0, const Vector3& v1,
	const Vector3& v2, const Vector4 color) {
	EmpSystems::GetInstance()->DrawTriangleColor(transform, v0, v1, v2, color);
}

void EmpEngine::SetWindowColor(Vector4 color) {
	EmpSystems::GetInstance()->SetWindowColor(color);
}

void EmpEngine::DrawPreModel(const Transform3d& transform, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle, const ModelData& modelData_) {
	EmpSystems::GetInstance()->DrawPreModel(transform, GraphHandle, modelData_);
}

LoaderManager EmpEngine::Resource() {
	return EmpSystems::GetInstance()->Resource();
}

void EmpEngine::DrawLight() {
	EmpSystems::GetInstance()->DrawLight();
}

ID3D12Device* EmpEngine::GetDevice() { return EmpSystems::GetInstance()->GetDevice(); }

void EmpEngine::DrawCompressModel(
	const Transform3d& transform,
	D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle,
	Model& model) {
	EmpSystems::GetInstance()->DrawCompressModel(transform, GraphHandle, model);
}

void EmpEngine::DrawSphere(const Transform3d& transform, const Vector4& color) {
	EmpSystems::GetInstance()->DrawSphere(transform, color);
}


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
