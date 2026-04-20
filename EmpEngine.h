#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>
#include<filesystem>
#include<fstream>
#include<chrono>

class EmpEngine {
	void WindowInitialize();
	void LogInitialize();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void __stdcall DebugPrint(LPCSTR lpOutputStrings);
	void Log(std::ofstream& os,const std::string& message);

	WNDCLASS wc_;
	RECT wrc_;
	HWND hwnd_;
	std::ofstream logStream_;
public:
	static void Initialize();
	static int ProcessMessage();
	static void Log(const std::string& message);
	static void Finalize();

private:
	//クライアントの領域サイズ
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	//初期化用のインスタンス
	static EmpEngine* instance_;

};