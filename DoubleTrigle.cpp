#include"DoubleTrigle.h"
#include"EmpEngine.h"
#include"externals/imgui/imgui.h"

void DoubleTrigle::Initialize() {
	monsterBall_ = EmpEngine::LoadTexture("resources/monsterBall.png");
	uvChecker_ = EmpEngine::LoadTexture("resources/uvChecker.png");
	oirano_ = EmpEngine::LoadTexture("resources/oirano.jpg");
	verticeTri_[0] = { -0.5f,-0.5f,0.0f};
	verticeTri_[1]= { 0.0f,0.5f,0.0f};
	verticeTri_[2] = { 0.5f,-0.5f,0.0f };
	trian1_ = {
		ReturnAllOne(),
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,-5.0f},
	};

	trian2_ = {
		ReturnAllOne(),
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,-5.0f},
	};

	GraphHandle1_ = uvChecker_;
	GraphHandle2_ = uvChecker_;
}

void DoubleTrigle::GUI() {
	const char* textureNames[] = { "uvChecker", "monsterBall", "oirano" };
	D3D12_GPU_DESCRIPTOR_HANDLE handles[] = { uvChecker_, monsterBall_, oirano_ };

	static int currentIndexAll = 0;
	static int currentIndex1 = 0;
	static int currentIndex2 = 0;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));  

	ImGui::BeginChild("Triangle1", { 0,150 }, false, 0);
	ImGui::SliderFloat3("Transrate", &trian1_.translate.x, -10.0f, 10.0f, "%.3f", 0);
	ImGui::SliderFloat3("Rotate", &trian1_.rotate.x, -10.0f, 10.0f, "%.3f", 0);
	ImGui::SliderFloat3("Scale", &trian1_.scale.x, -10.0f, 10.0f, "%.3f", 0);
	if (ImGui::Combo("GraphHandle1", &currentIndex1, textureNames, 3)) {
		GraphHandle1_ = handles[currentIndex1];
	}
	ImGui::EndChild();
	
	ImGui::BeginChild("Triangle2", { 0,150 }, false, 0);
	ImGui::SliderFloat3("Transrate", &trian2_.translate.x, -10.0f, 10.0f, "%.3f", 0);
	ImGui::SliderFloat3("Rotate", &trian2_.rotate.x, -10.0f, 10.0f, "%.3f", 0);
	ImGui::SliderFloat3("Scale", &trian2_.scale.x, -10.0f, 10.0f, "%.3f", 0);
	if (ImGui::Combo("GraphHandle2", &currentIndex2, textureNames, 3)) {
		GraphHandle2_ = handles[currentIndex2];
	}
	ImGui::EndChild();

	ImGui::PopStyleVar();

	if (ImGui::Combo("GraphHandleAll", &currentIndexAll, textureNames, 3)) {
		GraphHandle1_ = handles[currentIndexAll];
		GraphHandle2_ = handles[currentIndexAll];
	}

}

void DoubleTrigle::Draw() {
	EmpEngine::DrawTriangle(trian1_, verticeTri_[0], verticeTri_[1], verticeTri_[2], { 1.0f,1.0f,1.0f,1.0f }, GraphHandle1_);
	EmpEngine::DrawTriangle(trian2_, verticeTri_[0], verticeTri_[1], verticeTri_[2], { 1.0f,1.0f,1.0f,1.0f }, GraphHandle2_);
}