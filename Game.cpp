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
    debugCamera_.Initialize(SceneSystem::GetCamera()->GetCameraPosition(),SceneSystem::GetCamera()->GetCameraRotate());
	uvChecker = EmpEngine::LoadTexture("resources/uvChecker.png");
	monsterBall = EmpEngine::LoadTexture("resources/monsterBall.png");
	modelData_ = EmpEngine::LoadObjFile("resources", "axis.obj");
	Alarm01_ = EmpEngine::SoundLoadWave("C:/Windows/Media/Alarm01.wav");
}

void GameManager::Update() {
	colorBuff = SprColor * 255.0f;
	if (Input::GetInstance()->IsTriggerVk(MDK_ENTER)) {
		EmpEngine::PlayAudio(Alarm01_);
	}

	GuiHomeWork();
	UpdateHomeWork();
}

void GameManager::Draw() {
	DrawHomeWork();

	//最後
	EmpEngine::DrawLight();
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
	ImGui::Begin("Homework");
	ImGui::SliderAngle("Rotatex", &transform.rotate.x);
	ImGui::SliderAngle("Rotatey", &transform.rotate.y);
	ImGui::SliderAngle("Rotatez", &transform.rotate.z);
	ImGui::Checkbox("useMonsterBall", &useMonsterBall);
	EmpEngine::LightGUI();
	ImGui::End();


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
	debugCamera_.Update();
	SceneSystem::GetCamera()->SetCameraRotate(debugCamera_.GetRotate());
	SceneSystem::GetCamera()->SetCameraPosition(debugCamera_.GetTranslate());
	SceneSystem::GetCamera()->Update(windowWidth_, windowHeight_);
}