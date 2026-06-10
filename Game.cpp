#include"Game.h"
#include<time.h>
#include"EmpEngine.h"
#include"SceneSystem.h"
#include"Input.h"
#include"externals/imgui/imgui.h"


//先に初期化を入れておく
GameManager::GameManager() {
	srand(static_cast<unsigned int>(time(nullptr)));
}

void GameManager::Initialize() {
    camecon_.Initialize(SceneSystem::GetCamera()->GetCameraPosition(),SceneSystem::GetCamera()->GetCameraRotate());
	uvChecker = EmpEngine::LoadTexture("resources/uvChecker.png");
	monsterBall = EmpEngine::LoadTexture("resources/monsterBall.png");
}

void GameManager::Update() {
	colorBuff = SprColor * 255.0f;
	GuiHomeWork();
	UpdateHomeWork();
}

void GameManager::Draw() {
	DrawHomeWork();
}

void GameManager::GUI() {
#ifdef USE_IMGUI//GUIエリア☆（ECCジュニアのリズムで）

#endif 

}

void GameManager::DrawHomeWork() {
	EmpEngine::DrawSprite(transformSpr, 
		{ 0.0f,360.0f }, { 0.0f,0.0f }, { 640.0f,360.0f }, { 640.0f,0.0f },
		SprColor, uvChecker,uvTransformSpr);
	EmpEngine::DrawSphere(transform, { 1.0f,1.0f,1.0f,1.0f }, sphereHandle);
}

void GameManager::GuiHomeWork() {
#ifdef USE_IMGUI//GUIエリア☆（ECCジュニアのリズムで）

	ImGui::SliderFloat4("Material", &colorBuff.x, 0.0f, 255.0f, "%3f", 0);
	ImGui::SliderFloat3("Transform", &transformSpr.translate.x, 0.0f, 1280.0f, "%3f", 0);
	ImGui::Checkbox("useMonsterBall", &useMonsterBall);
	ImGui::DragFloat2("UVTranslate", &uvTransformSpr.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransformSpr.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransformSpr.rotate.z);
	EmpEngine::LightGUI();



#endif 

}

void GameManager::UpdateHomeWork() {
	if (Input::GetInstance()->IsReleaseVk(MDK_SPACE)) {

		if (useMonsterBall) {
			useMonsterBall = false;
		}
		else {
			useMonsterBall = true;
		}
	}

	if (useMonsterBall) {
		sphereHandle = monsterBall;
	}
	else {
		sphereHandle = uvChecker;
	}

	transform.rotate.y += 0.03f;
	SprColor = colorBuff / 255.0f;
	SceneSystem::GetCamera()->Update(windowWidth_, windowHeight_);
}