#include"ManagementLighting.h"
#include"externals/imgui/imgui.h"

void ManagementLighting::Initialize() {
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;
}

void ManagementLighting::GUI() {
#ifdef USE_IMGUI
	ImGui::SliderFloat4("color", &directionalLightData->color.x, 0.0f, 1.0f, "%.3f", 0);
	ImGui::SliderFloat3("direction", &directionalLightData->direction.x, -1.0f, 1.0f, "%.3f", 0);
	ImGui::SliderFloat("intensity", &directionalLightData->intensity, 0.0f, 1.0f, "%.3f", 0);

#endif // USE_IMGUI

}