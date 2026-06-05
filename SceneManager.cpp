#include "SceneManager.h"
#include"Input.h"
#include"Game.h"
#include"Title.h"
#include"Over.h"
#include"Clear.h"
#include"SceneSystem.h"
SceneManager::SceneManager() {
	SceneSystem::Bind(this);
	camera_ = std::make_unique<Camera3d>();
}

void SceneManager::Initialize(int windowWidth, int windowHeight, SceneName scene) {
	windowWidth_ = windowWidth;
	windowHeight_ = windowHeight;
	camera_->Initialize();
	SetScene(scene);
}

void SceneManager::Update() {
	// キー入力を受け取る
	Input::GetInstance()->InputAllUpdate();
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

void SceneManager::Process() {
	//更新処理
	Update();
	//描画処理
	Draw();
}
