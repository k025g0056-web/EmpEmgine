#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<cstdint>

class EmpEngine {
	void WindowInitialize();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void __stdcall DebugPrint(LPCSTR lpOutputStrings);
	void InitializeImpl();
	static EmpEngine& GetInstance();
	EmpEngine(const EmpEngine&) = delete;
	EmpEngine& operator=(const EmpEngine&) = delete;
	WNDCLASS wc_{};
	RECT wrc_{};
	HWND hwnd_{};
	EmpEngine();
public:
	static void Initialize();
	static int ProcessMessage();
	static void Finalize();

private:
	//クライアントの領域サイズ
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;


};