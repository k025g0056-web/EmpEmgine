#include "Mouse.h"
#include<cassert>

Mouse* Mouse::GetInstance() {
	static Mouse instance;
	return &instance;
}

void Mouse::Initialize(HWND hwnd,IDirectInput8* directInput) {
	directInput->CreateDevice(GUID_SysMouse, &mouse_, nullptr);

	HRESULT hr = mouse_->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(hr));

	hr = mouse_->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	assert(SUCCEEDED(hr));
}

void Mouse::Update() {
	HRESULT mouseHr = mouse_->Acquire();
	if (SUCCEEDED(mouseHr)) {
		preMouseState_ = mouseState_;
		mouse_->GetDeviceState(sizeof(mouseState_), &mouseState_);
		wheelDelta_ = mouseState_.lZ;
	}
}

void Mouse::Finalize() {
	if (mouse_) {
		mouse_->Unacquire();
		mouse_->Release();
		mouse_ = nullptr;
	}
}

bool Mouse::IsMousePress(MouseButton button) {
	return mouseState_.rgbButtons[static_cast<int>(button)] & 0x80;
}

bool Mouse::IsMouseTrigger(MouseButton button) {
	return (mouseState_.rgbButtons[static_cast<int>(button)] & 0x80)
		&& !(preMouseState_.rgbButtons[static_cast<int>(button)] & 0x80);
}

bool Mouse::IsMouseRelease(MouseButton button) {
	return !(mouseState_.rgbButtons[static_cast<int>(button)] & 0x80)
		&& (preMouseState_.rgbButtons[static_cast<int>(button)] & 0x80);
}