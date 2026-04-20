#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<cstdint>

class EmpEngine {
	void WindowInitialize();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void __stdcall DebugPrint(LPCSTR lpOutputStrings);

	WNDCLASS wc_;
	RECT wrc_;
	HWND hwnd_;

public:
	static void Initialize();
	static int ProcessMessage();
	static void Finalize();

private:
	//クライアントの領域サイズ
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	//初期化用のインスタンス
	static EmpEngine* instance_;

};