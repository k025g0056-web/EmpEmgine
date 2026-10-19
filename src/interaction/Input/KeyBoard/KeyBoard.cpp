#include "KeyBoard.h"
#include<cassert>

KeyBoard* KeyBoard::GetInstance() {
	static KeyBoard instance;
	return &instance;
}

void KeyBoard::Initialize(HWND hwnd, IDirectInput8* directInput) {
	DirectInput8Create(
		GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		nullptr);

	directInput->CreateDevice(GUID_SysKeyboard, &keyBord_, nullptr);

	HRESULT hr = keyBord_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	hr = keyBord_->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
}

void KeyBoard::Update() {
	
	GetKeyStateDI();
	GetKeyStateVk();
}

void KeyBoard::GetKeyStateDI() {
	HRESULT hr = keyBord_->Acquire();
	if (SUCCEEDED(hr)) {
		preKey_ = key_;
		keyBord_->GetDeviceState(static_cast<DWORD>(sizeof(key_)), key_.data());
	}
}

bool KeyBoard::IsPressVk(int key) {
	return nowKey_[key];
}

bool KeyBoard::IsTriggerVk(int key) {
	return nowKey_[key] && !prevKey_[key];
}

bool KeyBoard::IsReleaseVk(int key) {
	return !nowKey_[key] && prevKey_[key];
}

bool KeyBoard::IsPress(int key) {
	return (key_[key] & 0x80) != 0;
}

bool KeyBoard::IsTrigger(int key) {
	return (key_[key] & 0x80) && !(preKey_[key] & 0x80);
}

bool KeyBoard::IsRelease(int key) {
	return !(key_[key] & 0x80) && (preKey_[key] & 0x80);
}

void KeyBoard::GetKeyStateVk() {
	prevKey_ = nowKey_;
	for (int i = 0; i < KEY_MAX; i++) {
		key_[i] = (GetAsyncKeyState(i) & 0x8000) ? 1 : 0;
	}
}

void KeyBoard::Finalize() {
	if (keyBord_) {
		keyBord_->Unacquire();
		keyBord_->Release();
		keyBord_ = nullptr;
	}
}