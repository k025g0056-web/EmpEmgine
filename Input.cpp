#define DIRECTINPUT_VERSION 0x0800
#include"Input.h"
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include <Windows.h>
#include<cassert>

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

bool Input::IsPress(int key) {
	return (key_[key] & 0x80) != 0;
}

bool Input::IsTrigger(int key) {
	return (key_[key] & 0x80) && !(prekey[key] & 0x80);
}

bool Input::IsRelease(int key) {
	return !(key_[key] & 0x80) && (prekey[key] & 0x80);
}

void Input::GetKeyStateVk(char*key) {
	for (int i = 0; i < KEY_MAX;i++) {
		key[i] = (GetAsyncKeyState(i) & 0x8000) ? 1 : 0;
	}
}

void Input::InputAllUpdate() {
	keybord_->Acquire();
	memcpy(prekey, key_, KEY_MAX);
	keybord_->GetDeviceState(sizeof(key_), key_);
	keybord_->GetDeviceState(sizeof(prekey), prekey);


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

	HRESULT hr = keybord_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	hr = keybord_->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE|DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

}