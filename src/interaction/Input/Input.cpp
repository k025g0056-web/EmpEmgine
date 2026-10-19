#define DIRECTINPUT_VERSION 0x0800
#include"Input.h"
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xinput.lib")
#include <Windows.h>
#include<cassert>
#include"KeyBoard/KeyBoard.h"
#include"Mouse/Mouse.h"
#include"Pad/Pad.h"

Input* Input::GetInstance() {
	static Input instance_;
	return &instance_;
}
KeyBoard* Input::GetKeyBoard() { return KeyBoard::GetInstance(); }
Mouse* Input::GetMouse() { return Mouse::GetInstance(); }
Pad* Input::GetPad() { return Pad::GetInstance(); }

void Input::InputAllUpdate() {
	GetKeyBoard()->Update();
	GetMouse()->Update();
	GetPad()->Update();
}

void Input::Initialize(HWND hwnd) {
	GetKeyBoard()->Initialize(hwnd, directInput_);
	GetMouse()->Initialize(hwnd, directInput_);
}

void Input::Finalize() {
	GetKeyBoard()->Finalize();
	GetMouse()->Finalize();
	if (directInput_) {
		directInput_->Release();
		directInput_ = nullptr;
	}

}