#pragma once
#pragma once
#include"Scene/Base/SceneBase.h"
#include<memory>
#include"appObj/Camera/Camera3d/Camera3d.h"

enum class SceneName {
	Title,
	Play,
	Clear,
	over
};

class SceneManager {
	std::unique_ptr<Scene> scene_;
	std::unique_ptr<Camera3d> camera_;
	int windowWidth_ = 0;
	int windowHeight_ = 0;
	
	void Update();

	void Draw();
public:
	SceneManager();
	~SceneManager() = default;

	void Initialize(int windowWidth, int windowHeight,SceneName scene);

	void Process();

	bool EndManagement();

	void SetScene(SceneName scene);

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	Camera3d* GetCamera() { return camera_.get(); }
};
