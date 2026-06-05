#pragma once

#include <array>
#include <cstdint>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include"KeyDiffine.h"

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
	char prevKey_[KEY_MAX];

	/// <summary>
	/// 今フレームのキーの状態
	/// </summary>
	char nowKey_[KEY_MAX];

	/// <summary>
	/// キーの状態を更新します(VK)
	/// </summary>
	/// <param name="key"></param>
	void GetKeyStateVk(char* key);

	/// <summary>
	/// インプットの変数です
	/// </summary>
	IDirectInput8* directInput_ = nullptr;

	/// <summary>
	/// 仮想キーボード
	/// </summary>
	IDirectInputDevice8* keybord_ = nullptr;

	/// <summary>
	/// マウスのスクロールの値
	/// </summary>
	int wheelDelta_ = 0;

	/// <summary>
	/// ホイールの動きを測定します
	/// </summary>
	void WheelReset() { wheelDelta_ = 0; }
};