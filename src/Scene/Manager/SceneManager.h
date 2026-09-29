#pragma once
#pragma once
#include"Scene/Base/SceneBase.h"
#include<memory>
#include"appObj/Camera/Camera3d/Camera3d.h"
#include"Internal/ImGui/SetRender/SetRenderGui.h"

enum class SceneName {
	Title,
	Play,
	Clear,
	over
};

class SceneManager {
	std::unique_ptr<Scene> scene_;
	std::unique_ptr<Camera3d> camera_;
	SetRenderGui setRender_;
	int sceneName_ = 1;
	const char* table[4]{
		"title",
		"play",
		"Clear",
		"over"
	};

	void Gui();

	int windowWidth_ = 0;
	int windowHeight_ = 0;
	


	SceneManager();
	~SceneManager() = default;
public:
	
	static SceneManager* GetInstance();

	void Initialize(int windowWidth, int windowHeight,SceneName scene);


	bool EndManagement();

	void Update();

	void Draw();

	void SetScene(SceneName scene);

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	Camera3d* GetCamera() { return camera_.get(); }
};
