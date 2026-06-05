#include"SceneSystem.h"
#include "SceneManager.h"

static SceneManager* g_manager = nullptr;

void SceneSystem::Bind(SceneManager* m) {
	g_manager = m;
}

void SceneSystem::Set(SceneName name) {
	if (g_manager) {
		g_manager->SetScene(name);
	}
}

Camera3d* SceneSystem::GetCamera() {
	if (g_manager) {
		return g_manager->GetCamera();
	} else {
		return 0;
	}
}
