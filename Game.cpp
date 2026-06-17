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
	modelData_ = EmpEngine::LoadObjFile("resources", "axis.obj");
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
	/*EmpEngine::DrawSprite(transformSpr,
		{ 0.0f,360.0f }, { 0.0f,0.0f }, { 640.0f,360.0f }, { 640.0f,0.0f },
		SprColor, uvChecker,uvTransformSpr);*/
	//EmpEngine::DrawSphere(transform, { 1.0f,1.0f,1.0f,1.0f }, sphereHandle);

	EmpEngine::DrawPreModel(transform, sphereHandle, modelData_);
}

void GameManager::GuiHomeWork() {
#ifdef USE_IMGUI//GUIエリア☆（ECCジュニアのリズムで）

	ImGui::SliderAngle("Rotatex", &transform.rotate.x);
	ImGui::SliderAngle("Rotatey", &transform.rotate.y);
	ImGui::SliderAngle("Rotatez", &transform.rotate.z);
	ImGui::Checkbox("useMonsterBall", &useMonsterBall);
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

	//transform.rotate.y += 0.03f;
	SprColor = colorBuff / 255.0f;
	SceneSystem::GetCamera()->Update(windowWidth_, windowHeight_);
}