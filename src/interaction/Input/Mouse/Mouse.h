#pragma once
#include"MouseButton.h"
#include<dinput.h>
class Mouse {

	IDirectInputDevice8* mouse_ = nullptr;
	DIMOUSESTATE mouseState_{};
	DIMOUSESTATE preMouseState_{};
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