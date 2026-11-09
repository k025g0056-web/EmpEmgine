#pragma once
#include"Scene/Base/SceneBase.h"

class Clear :public Scene {
public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

	Clear();
	~Clear() = default;
	Clear(const Clear&) = delete;
	Clear& operator=(const Clear&) = delete;
};