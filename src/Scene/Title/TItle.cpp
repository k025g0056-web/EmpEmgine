#include"Title.h"
#include"interaction/Input/Input.h"
#include"Scene/wrapper/SceneSystem.h"
#include "Scene/Manager/SceneManager.h"
#include "core/wrapper/EmpEngine.h"
Title::Title() {}

void Title::Initialize() {
	model_ = EmpEngine::Resource().Model().LoadObjFile("Title", "Title");
}

void Title::Update() {
	if (Input::GetInstance()->IsPress(DIK_SPACE)) {
		SceneSystem::Set(SceneName::Play);
	}

	SceneSystem::GetCamera()->SetCameraPosition({ transform_.translate.x,transform_.translate.y,transform_.translate.z - 10.0f });
	SceneSystem::GetCamera()->Update(windowWidth_,windowHeight_) ;
}

void Title::Draw() {
	EmpEngine::Draw().DrawModel(model_, transform_, EmpEngine::GetWhite1x1(), Vector4{ 1.0f,1.0f,1.0f,1.0f });
}