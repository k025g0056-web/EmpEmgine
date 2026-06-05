#include"ColorTriangle.h"
#include"externals/imgui/imgui.h"
#include"EmpEngine.h"

void ColorTriangle::initialize() {
	vertice_[0] = { 0.0f,  0.5f, 0.0f };
	vertice_[1] = { 0.5f, -0.5f, 0.0f };
	vertice_[2] = { -0.5f, -0.5f, 0.0f };
	color = { 1.0f,1.0f,1.0f,1.0f };
}

void ColorTriangle::GUI() {
#ifdef USE_IMGUI
	ImGui::SliderFloat3("Vertice1", &vertice_[0].x, -10.0f, 10.0f, "%.3f", 0);
	ImGui::SliderFloat3("Vertice2", &vertice_[1].x, -10.0f, 10.0f, "%.3f", 0);
	ImGui::SliderFloat3("Vertice3", &vertice_[2].x, -10.0f, 10.0f, "%.3f", 0);
	ImGui::ColorEdit4("Color", &color.x, 0);
#endif // USE_IMGUI

}

void ColorTriangle::Draw() {
	Transform3d trans{};
	EmpEngine::DrawTriangleColor(trans,vertice_[0], vertice_[1], vertice_[2], color);
}
