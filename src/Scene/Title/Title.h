#pragma once
#include"Scene/Base/SceneBase.h"
#include"DataModel/ModelData.h"
#include"DataModel/TransForm3d.h"
enum class SceneName;


class Title :public Scene {
	ModelData model_;
	Transform3d transform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f },{0.0f,0.0f,0.0f} };


public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

	Title();
	~Title() = default;
	Title(const Title&) = delete;
	Title& operator=(const Title&) = delete;
};