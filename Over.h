#pragma once
#include"SceneBase.h"

class Over :public Scene {

public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

	Over();
	~Over() = default;
	Over(const Over&) = delete;
	Over& operator=(const Over&) = delete;
};