#pragma once
#include"Scene/Base/SceneBase.h"

class Title :public Scene {

public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

	Title();
	~Title() = default;
	Title(const Title&) = delete;
	Title& operator=(const Title&) = delete;
};