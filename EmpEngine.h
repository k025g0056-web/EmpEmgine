#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>



class EmpEngine {
	void WindowInitialize();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void __stdcall DebugPrint(LPCSTR lpOutputStrings);
	void UpdateImpl();
	static EmpEngine& GetInstance();
	void InitializeImpl();
	std::wstring ConvertString(const std::string& str);
	std::string ConvertString(const std::wstring& str);
	void LogImpl(const std::wstring& message);

	WNDCLASS wc_;
	RECT wrc_;
	HWND hwnd_;

public:
	static void Initialize();
	static int ProcessMessage();
	static void Log(const std::string& message);
	static void Log(const std::wstring& message);
	static void Finalize();
	static void Update();

private:
	//クライアントの領域サイズ
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

};