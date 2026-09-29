#pragma once
#include"DataModel/Vector3.h"

class DebugCamera {
	Vector3 translate_{};
	Vector3 rotate_{};

	bool onCamera = false;

	int mouseX{};
	int mouseY{};
	int preMouseX{};
	int preMouseY{};

	const float kSpeed = 0.01f;
	Vector3 up{};
	Vector3 right{};
	Vector3 forward{};
	void RotateToMat();
public:
	void Initialize(Vector3 translate, Vector3 rotate);
	void Update();
	Vector3 GetTranslate() { return translate_; }
	Vector3 GetRotate() { return rotate_; }
};