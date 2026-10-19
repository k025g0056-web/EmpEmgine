#pragma once
#include<Windows.h>
#include"MouseButton.h"
#include<dinput.h>
class Mouse {

	IDirectInputDevice8* mouse_ = nullptr;
	DIMOUSESTATE2 mouseState_{};
	DIMOUSESTATE2 preMouseState_{};
	int wheelDelta_ = 0;
public:
	static Mouse* GetInstance();
	void Initialize(HWND hwnd, IDirectInput8* directInput);
	void Update();
	void Finalize();
	int GetMouseWheel()const { return wheelDelta_; }
	bool IsWheelUp()const { return wheelDelta_ > 0; }
	bool IsWheelDown()const { return wheelDelta_ < 0; }
	void SetWheel(int delta) { wheelDelta_ = delta; }
	bool IsMousePress(MouseButton button);
	bool IsMouseTrigger(MouseButton button);
	bool IsMouseRelease(MouseButton button);

private:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	void WheelReset() { wheelDelta_ = 0; }
};