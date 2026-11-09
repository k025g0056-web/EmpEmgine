#include"SceneSystem.h"
#include "Scene/Manager/SceneManager.h"

void SceneSystem::Set(SceneName name) {
	SceneManager::GetInstance()->SetScene(name);
}

Camera3d* SceneSystem::GetCamera() {
	return SceneManager::GetInstance()->GetCamera();
}
