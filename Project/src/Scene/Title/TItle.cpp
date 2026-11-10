#include"Title.h"
#include"interaction/Input/Input.h"
#include"Scene/wrapper/SceneSystem.h"
#include "Scene/Manager/SceneManager.h"
#include "core/wrapper/EmpEngine.h"
Title::Title() {}

void Title::Initialize() {
	
}

void Title::Update() {
	if (Input::GetInstance()->GetKeyBoard()->IsPress(DIK_SPACE)) {
		SceneSystem::Set(SceneName::Play);
	}

	SceneSystem::GetCamera()->SetCameraPosition({ transform_.translate.x,transform_.translate.y,transform_.translate.z - 10.0f });
}

void Title::Draw() {
}