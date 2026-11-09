#include "Pad.h"

Pad* Pad::GetInstance() {
	static Pad instance;
	return &instance;
}

void Pad::Update() {
	prePadState_ = padState_;
	XInputGetState(0, &padState_);
}

bool Pad::IsPadPress(PadButton button) {
	return (padState_.Gamepad.wButtons & static_cast<WORD>(button)) != 0;
}

bool Pad::IsPadTrigger(PadButton button) {
	return (padState_.Gamepad.wButtons & static_cast<WORD>(button)) &&
		!(prePadState_.Gamepad.wButtons & static_cast<WORD>(button));
}

bool Pad::IsPadRelease(PadButton button) {
	return !(padState_.Gamepad.wButtons & static_cast<WORD>(button)) &&
		(prePadState_.Gamepad.wButtons & static_cast<WORD>(button));
}

float Pad::GetLeftStickX() {
	return padState_.Gamepad.sThumbLX / 32767.0f;
}

float Pad::GetLeftStickY() {
	return padState_.Gamepad.sThumbLY / 32767.0f;
}

float Pad::GetLeftTrigger() {
	return padState_.Gamepad.bLeftTrigger / 255.0f;
}

float Pad::GetRightTrigger() {
	return padState_.Gamepad.bRightTrigger / 255.0f;
}