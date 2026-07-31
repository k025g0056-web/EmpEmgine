#define DIRECTINPUT_VERSION 0x0800
#include"Input.h"
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xinput.lib")
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
	return (key_[key] & 0x80) && !(preKey[key] & 0x80);
}

bool Input::IsRelease(int key) {
	return !(key_[key] & 0x80) && (preKey[key] & 0x80);
}

void Input::GetKeyStateVk(bool*key) {
	for (int i = 0; i < KEY_MAX;i++) {
		key[i] = (GetAsyncKeyState(i) & 0x8000) ? 1 : 0;
	}
}

void Input::InputAllUpdate() {
	HRESULT hr = keyBord_->Acquire();
	if (FAILED(hr)) return;

	memcpy(preKey, key_, KEY_MAX);
	keyBord_->GetDeviceState(sizeof(key_), key_);

	memcpy(prevKey_, nowKey_, KEY_MAX);
	GetKeyStateVk(nowKey_);

	// マウス
	mouse_->Acquire();
	preMouseState_ = mouseState_;
	mouse_->GetDeviceState(sizeof(mouseState_), &mouseState_);
	wheelDelta_ = mouseState_.lZ;

	// XInput
	prePadState_ = padState_;
	XInputGetState(0, &padState_);
}

void Input::Initialize(HWND hwnd) {
	DirectInput8Create(
		GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput_,
		nullptr);

	directInput_->CreateDevice(GUID_SysKeyboard, &keyBord_, nullptr);

	HRESULT hr = keyBord_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	hr = keyBord_->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE|DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

	directInput_->CreateDevice(GUID_SysMouse, &mouse_, nullptr);

	hr = mouse_->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(hr));

	hr = mouse_->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	assert(SUCCEEDED(hr));

}

void Input::Finalize() {
	if (keyBord_) {
		keyBord_->Unacquire();
		keyBord_->Release();
		keyBord_ = nullptr;
	}

	if (directInput_) {
		directInput_->Release();
		directInput_ = nullptr;
	}

	if (mouse_) {
		mouse_->Release();
		mouse_ = nullptr;
	}
}

bool Input::IsMousePress(MouseButton button){
	return mouseState_.rgbButtons[static_cast<int>(button)] & 0x80;
}

bool Input::IsMouseTrigger(MouseButton button){
	return (mouseState_.rgbButtons[static_cast<int>(button)] & 0x80)
		&& !(preMouseState_.rgbButtons[static_cast<int>(button)] & 0x80);
}

bool Input::IsMouseRelease(MouseButton button){
	return !(mouseState_.rgbButtons[static_cast<int>(button)] & 0x80)
		&& (preMouseState_.rgbButtons[static_cast<int>(button)] & 0x80);
}

bool Input::IsPadPress(PadButton button){
	return (padState_.Gamepad.wButtons & static_cast<WORD>(button)) != 0;
}

bool Input::IsPadTrigger(PadButton button){
	return (padState_.Gamepad.wButtons & static_cast<WORD>(button)) &&
		!(prePadState_.Gamepad.wButtons & static_cast<WORD>(button));
}

bool Input::IsPadRelease(PadButton button){
	return !(padState_.Gamepad.wButtons & static_cast<WORD>(button)) &&
		(prePadState_.Gamepad.wButtons & static_cast<WORD>(button));
}

float Input::GetLeftStickX(){
	return padState_.Gamepad.sThumbLX / 32767.0f;
}

float Input::GetLeftStickY(){
	return padState_.Gamepad.sThumbLY / 32767.0f;
}

float Input::GetLeftTrigger(){
	return padState_.Gamepad.bLeftTrigger / 255.0f;
}

float Input::GetRightTrigger(){
	return padState_.Gamepad.bRightTrigger / 255.0f;
}