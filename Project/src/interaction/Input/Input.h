#pragma once
#include <Windows.h>
#include <dinput.h>
#include"KeyDiffine.h"
#include"KeyBoard/KeyBoard.h"
#include"Mouse/Mouse.h"
#include"Pad/Pad.h"

class Input {
public:
	/// <summary>
	/// インスタンスを取得する
	/// シングルトンにしてある
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	KeyBoard* GetKeyBoard();
	Mouse* GetMouse();
	Pad* GetPad();

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
	/// ここでデリートする
	/// </summary>
	void Finalize();

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
	/// インプットの変数です
	/// </summary>
	IDirectInput8* directInput_ = nullptr;
};