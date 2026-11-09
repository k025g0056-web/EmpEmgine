#pragma once
#include"PadButton.h"

class Pad{
	XINPUT_STATE padState_{};
	XINPUT_STATE prePadState_{};
public:
	static Pad* GetInstance();
	void Update();

	bool IsPadPress(PadButton button);
	bool IsPadTrigger(PadButton button);
	bool IsPadRelease(PadButton button);
	float GetLeftStickX();
	float GetLeftStickY();
	float GetLeftTrigger();
	float GetRightTrigger();
private:
	Pad() = default;
	Pad(const Pad&) = delete;
	Pad& operator=(const Pad&) = delete;
};

