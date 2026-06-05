#include"Input.h"
#include <Windows.h>
#include <dinput.h>


Input* Input::GetInstance() {
	static Input instance_;
	return &instance_;
}

bool Input::IsPressVk(int key) {
	return nowKey_[key];
}

bool Input::IsTriggerVk(int key) {
	return nowKey_[key] && !prevKey_[key];
}

bool Input::IsReleaseVk(int key) {
	return !nowKey_[key] && prevKey_[key];
}

void Input::GetKeyStateVk(char*key) {
	for (int i = 0; i < KEY_MAX;i++) {
		key[i] = (GetAsyncKeyState(i) & 0x8000) ? 1 : 0;
	}
}

void Input::InputAllUpdate() {
	WheelReset();
	memcpy(prevKey_, nowKey_, KEY_MAX);
	GetKeyStateVk(nowKey_);
}

void Input::Initialize(HWND hwnd) {
	DirectInput8Create(
		GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput_,
		nullptr);

	directInput_->CreateDevice(GUID_SysKeyboard, &keybord_, nullptr);

	keybord_->SetDataFormat(&c_dfDIKeyboard);
	keybord_->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	keybord_->Acquire();
}