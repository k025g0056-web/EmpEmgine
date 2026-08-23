#include "SceneManager.h"
#include"interaction/Input/Input.h"
#include"Scene/Game/Game.h"
#include"Scene/Title/Title.h"
#include"Scene/Clear/Clear.h"
#include"Scene/Over/Over.h"
#include"Scene/wrapper/SceneSystem.h"
#include"externals/imgui/imgui.h"

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return&instance;
}

SceneManager::SceneManager() {
	camera_ = std::make_unique<Camera3d>();
}

void SceneManager::Initialize(int windowWidth, int windowHeight, SceneName scene) {
	windowWidth_ = windowWidth;
	windowHeight_ = windowHeight;
	camera_->Initialize();
	SetScene(scene);
}

void SceneManager::Update() {
#ifdef USE_IMGUI
	setRender_.Update();
	Gui();
#endif
	scene_->Update();
}

void SceneManager::Draw() {
	scene_->Draw();
}

bool SceneManager::EndManagement() {
	// ESCキーが押されたらループを抜ける
	return (Input::GetInstance()->IsReleaseVk(MDK_ESCAPE));
}

void SceneManager::SetScene(SceneName scene) {

	sceneName_ = static_cast<int>(scene);

	switch (scene) {
	case SceneName::Title:
		scene_ = std::make_unique<Title>();
		break;
	case SceneName::Play:
		scene_ = std::make_unique<GameManager>();
		break;
	case SceneName::Clear:
		scene_ = std::make_unique<Clear>();
		break;
	case SceneName::over:
		scene_ = std::make_unique<Over>();
		break;
	default:
		break;
	}

	scene_->SetWindowWidth(windowWidth_);
	scene_->SetWindowHeight(windowHeight_);
	scene_->Initialize();
}

void SceneManager::Gui() {
#ifdef USE_IMGUI
	ImGui::Begin("SceneChange");

	if (ImGui::Combo("Scene", &sceneName_, table, 4)) {
		SetScene(static_cast<SceneName>(sceneName_));
	}

	ImGui::End();
#endif
}
