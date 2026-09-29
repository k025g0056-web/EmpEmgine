#pragma once

#include <array>
#include <cstdint>

#include <dinput.h>

#include"KeyDiffine.h"

#include <Xinput.h>

enum class MouseButton{
	Left,
	Right,
	Middle,
	Side1,
	Side2
};

enum class PadButton : WORD

{
	PAD_UP = XINPUT_GAMEPAD_DPAD_UP,
	PAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	PAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	PAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,

	PAD_START = XINPUT_GAMEPAD_START,
	PAD_BACK = XINPUT_GAMEPAD_BACK,

	PAD_LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	PAD_RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,

	PAD_LS = XINPUT_GAMEPAD_LEFT_THUMB,
	PAD_RS = XINPUT_GAMEPAD_RIGHT_THUMB,

	PAD_A = XINPUT_GAMEPAD_A,
	PAD_B = XINPUT_GAMEPAD_B,
	PAD_X = XINPUT_GAMEPAD_X,
	PAD_Y = XINPUT_GAMEPAD_Y
};

/// <summary>
/// キーの最大数
/// </summary>
static constexpr int KEY_MAX = 256;

class Input {
public:
	/// <summary>
	/// インスタンスを取得する
	/// シングルトンにしてある
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	/// <summary>
	/// 押した瞬間(VK,MDK)
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsTriggerVk(int key);

	/// <summary>
	///押してる間はon(VK,MDK)
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsPressVk(int key);
	
	/// <summary>
	/// 離した瞬間のみon(VK,MDK)
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsReleaseVk(int key);

	/// <summary>
	/// 押した瞬間(VK,MDK)
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsTrigger(int key);

	/// <summary>
	///押してる間はon(VK,MDK)
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsPress(int key);

	/// <summary>
	/// 離した瞬間のみon(VK,MDK)
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsRelease(int key);


	/// <summary>
	/// インプットの状態更新
	/// </summary>
	void InputAllUpdate();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="hwnd"></param>
	void Initialize(HWND hwnd);

	/// <summary>
	/// マウスホイールの値を使いたいときに使います
	/// </summary>
	/// <returns></returns>
	int GetMouseWheel()const { return wheelDelta_; }

	/// <summary>
	/// ホイールが上に廻っているかを取ります
	/// </summary>
	/// <returns></returns>
	bool IsWheelUp()const { return wheelDelta_ > 0; }

	/// <summary>
	/// ホイールが下に廻っているかを取ります
	/// </summary>
	/// <returns></returns>
	bool IsWheelDown()const { return wheelDelta_ < 0; }

	/// <summary>
	/// 運動量を設定します
	/// </summary>
	/// <param name="delta"></param>
	void SetWheel(int delta) { wheelDelta_ = delta; }

	/// <summary>
	/// ここでデリートする
	/// </summary>
	void Finalize();

	bool IsMousePress(MouseButton button);

	bool IsMouseTrigger(MouseButton button);

	bool IsMouseRelease(MouseButton button);

	bool IsPadPress(PadButton button);

	bool IsPadTrigger(PadButton button);

	bool IsPadRelease(PadButton button);

	float GetLeftStickX();

	float GetLeftStickY();

	float GetLeftTrigger();

	float GetRightTrigger();


private:
	/// <summary>
	/// ニュー禁止
	/// </summary>
	Input() = default;

	/// <summary>
	/// コピー禁止
	/// </summary>
	/// <param name=""></param>
	Input(const Input&) = delete;

	/// <summary>
	/// 代入禁止
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Input& operator=(const Input&) = delete;

	/// <summary>
	/// 前フレームのキーの状態
	/// </summary>
	bool prevKey_[KEY_MAX];

	/// <summary>
	/// 今フレームのキーの状態
	/// </summary>
	bool nowKey_[KEY_MAX];

	/// <summary>
	/// キーの状態を更新します(VK)
	/// </summary>
	/// <param name="key"></param>
	void GetKeyStateVk(bool* key);

	/// <summary>
	/// インプットの変数です
	/// </summary>
	IDirectInput8* directInput_ = nullptr;

	/// <summary>
	/// 仮想キーボード
	/// </summary>
	IDirectInputDevice8* keyBord_ = nullptr;

	/// <summary>
	/// マウスのスクロールの値
	/// </summary>
	int wheelDelta_ = 0;

	/// <summary>
	/// ホイールの動きを測定します
	/// </summary>
	void WheelReset() { wheelDelta_ = 0; }

	BYTE key_[KEY_MAX]{};
	BYTE preKey[KEY_MAX]{};

	// マウス
	IDirectInputDevice8* mouse_ = nullptr;

	DIMOUSESTATE2 mouseState_{};
	DIMOUSESTATE2 preMouseState_{};

	// Xboxコントローラー
	XINPUT_STATE padState_{};
	XINPUT_STATE prePadState_{};
};