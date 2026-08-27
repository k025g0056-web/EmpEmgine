
#include <d3d12.h>
#include"Datamodel/BlendMode.h"
#include "SetRenderGui.h"
#include"core/wrapper/EmpEngine.h"
#include"externals/imgui/imgui.h"


void SetRenderGui::Update() {
	SetBlend();
	SetRasterizer();
	SetDepthStencil();

	EmpEngine::GeneratePSO();
}

void SetRenderGui::SetBlend() {
	ImGui::Begin("Blend");

	if (ImGui::Combo("BlendMode", &blendMode_, blendItems, 6)) {
		EmpEngine::SetBlendMode(static_cast<BlendMode>(blendMode_));
	}

	ImGui::End();


}

void SetRenderGui::SetRasterizer() {
	ImGui::Begin("Rasterizer");

	if (ImGui::Combo("cullMode", &cullMode_, cullItems, 3)) {
		EmpEngine::SetRasterizer(static_cast<D3D12_CULL_MODE>(cullMode_+1), static_cast<D3D12_FILL_MODE>(fillMode_ + 2));
	}

	if (ImGui::Combo("fillMode", &fillMode_, fillItems, 2)) {
		EmpEngine::SetRasterizer(static_cast<D3D12_CULL_MODE>(cullMode_+1), static_cast<D3D12_FILL_MODE>(fillMode_ + 2));
	}

	ImGui::End();
}

void SetRenderGui::SetDepthStencil() {
	ImGui::Begin("DepthStencil");

	if (ImGui::Combo("depthEnable",&isEnable_,flagTable,2)) {
		EmpEngine::SetDepthStencil(static_cast<bool>(isEnable_), static_cast<D3D12_DEPTH_WRITE_MASK>(WriteMask_), static_cast<D3D12_COMPARISON_FUNC>(comparisonFunc_));
	}

	if (ImGui::Combo("depthWriteMask", &WriteMask_, maskItem_, 2)) {
		EmpEngine::SetDepthStencil(static_cast<bool>(isEnable_), static_cast<D3D12_DEPTH_WRITE_MASK>(WriteMask_), static_cast<D3D12_COMPARISON_FUNC>(comparisonFunc_));
	}

	if (ImGui::Combo("comparisonFunction", &comparisonFunc_, funcItem_, 9)) {
		EmpEngine::SetDepthStencil(static_cast<bool>(isEnable_), static_cast<D3D12_DEPTH_WRITE_MASK>(WriteMask_), static_cast<D3D12_COMPARISON_FUNC>(comparisonFunc_));
	}

	ImGui::End();

}