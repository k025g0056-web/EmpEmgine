#pragma once
#include"Camera3d.h"
class SceneManager;
enum class SceneName;

class SceneSystem {
public:
	static void Set(SceneName name);
	static void Bind(SceneManager* m);
	static Camera3d* GetCamera();
};
