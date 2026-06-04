#include"Game.h"
#include<time.h>
#include"EmpEngine.h"
#include"SceneSystem.h"
#include"Input.h"

//先に初期化を入れておく
GameManager::GameManager() {
	srand(static_cast<unsigned int>(time(nullptr)));
}

void GameManager::Initialize() {
	uvChecker = EmpEngine::LoadTexture("resources/uvChecker.png");
	monsterBall = EmpEngine::LoadTexture("resources/monsterBall.png");

}

void GameManager::Update() {
	Vector4 color = SprColor * 255.0f;

#ifdef USE_IMGUI
	//GUIエリア☆（ECCジュニアのリズムで）
	ImGui::SliderFloat4("Material", &color.x, 0.0f, 255.0f, "%3f", 0);
	ImGui::SliderFloat3("Transform", &transformSpr.translate.x, 0.0f, 1280.0f, "%3f", 0);
	ImGui::Checkbox("useMonsterBall", &useMonsterBall);
	EmpEngine::LightGUI();
#endif // USE_IMGUI
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
	SprColor = color / 255.0f;
	SceneSystem::GetCamera()->Update(windowWidth_, windowHeight_);
}

void GameManager::Draw() {
	DrawHomeWork();
}

void GameManager::DrawHomeWork() {
	EmpEngine::DrawSprite(transformSpr, { 0.0f,360.0f }, { 0.0f,0.0f }, { 640.0f,360.0f }, { 640.0f,0.0f }, SprColor, uvChecker);
	EmpEngine::DrawSphere(transform, { 1.0f,1.0f,1.0f,1.0f }, sphereHandle);
}