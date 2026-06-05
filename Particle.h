#pragma once
#include"TransForm3d.h"
#include"Vector4.h"
class Particle {
	bool isPlayBack ;
	float time ;　
	static const int maxAmount = 100;
	float fallSpeed[maxAmount]{};
	float swaySpeed[maxAmount]{};
	float swayAmount[maxAmount]{};
	float swayOffset[maxAmount]{};
	float length;
	float startY = 2.5f;
	Transform3d transform[maxAmount];
	Vector3 startPositions[maxAmount];
	Vector3 verticeTri_[3];
	Vector4 color;
public:
	void Initialize();

	void GUI();

	void Draw();
};