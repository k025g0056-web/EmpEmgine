#pragma once
#include <array>
#include <dinput.h>
#include<wrl.h>

/// <summary>
/// キーの最大数
/// </summary>
static constexpr int KEY_MAX = 256;

class KeyBoard{
	std::array<bool, KEY_MAX>prevKey_;
	std::array<bool, KEY_MAX>nowKey_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyBord_ = nullptr;
	std::array<BYTE, KEY_MAX>key_;
	std::array<BYTE, KEY_MAX>preKey_;


public:
	static KeyBoard* GetInstance();
	void Initialize(HWND hwnd, IDirectInput8* directInput);
	void Update();
	void Finalize();
	bool IsTriggerVk(int key);
	bool IsPressVk(int key);
	bool IsReleaseVk(int key);
	bool IsTrigger(int key);
	bool IsPress(int key);
	bool IsRelease(int key);
private:
	KeyBoard() = default;
	KeyBoard(const KeyBoard&) = delete;
	KeyBoard& operator=(const KeyBoard&) = delete;
	void GetKeyStateDI();
	void GetKeyStateVk();


};

