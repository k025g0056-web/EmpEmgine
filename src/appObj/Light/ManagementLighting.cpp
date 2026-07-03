#include"ManagementLighting.h"
#include"externals/imgui/imgui.h"
#include"Internal/DX12Mechanics/DX12Mechanics.h"

void ManagementLighting::Initialize(ID3D12Device* device) {
	directionalLightResource_ = DX12Mechanics::CreateBufferResource(device, sizeof(DirectionalLight), 256);
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;
}

void ManagementLighting::GUI() {
#ifdef USE_IMGUI
	ImGui::Begin("Light");
	ImGui::SliderFloat4("color", &directionalLightData->color.x, 0.0f, 1.0f, "%.3f", 0);
	ImGui::SliderFloat3("direction", &directionalLightData->direction.x, -1.0f, 1.0f, "%.3f", 0);
	ImGui::SliderFloat("intensity", &directionalLightData->intensity, 0.0f, 1.0f, "%.3f", 0);
	ImGui::End();
#endif // USE_IMGUI

}

void ManagementLighting::DrawCall(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
}

void ManagementLighting::Release() {
	if (directionalLightResource_) {
		directionalLightResource_->Release();
		directionalLightResource_ = nullptr;
		directionalLightData = nullptr;
	}
}