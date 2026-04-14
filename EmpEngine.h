#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>
#include<filesystem>
#include<fstream>
#include<chrono>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class EmpEngine {
	void WindowInitialize();
	void LogInitialize();
	void DXGIInitialize();
	void DecideAdapter();
	void GenerateDevice();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void CALLBACK DebugPrint(LPCSTR lpOutputStrings);
	void Log(std::ofstream& os,const std::string& message);

	WNDCLASS wc_;
	RECT wrc_;
	HWND hwnd_;

	//HRESULはWindows系のエラーコードであり、
	//関数が成功したかどうかをSucceedマクロで判定できる
	HRESULT hr_;

public:
	static void Initialize();
	static int ProcessMessage();
	static void Log(const std::string& message);
	static void Log(const std::wstring& message);
	static void Finalize();

private:
	//クライアントの領域サイズ
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	//初期化用のインスタンス
	static EmpEngine* instance_;

	//DXGIファクトリーの作成
	IDXGIFactory7* dxgiFactory_ = nullptr;

	//使用するアダプタ用の変数。最初にnullptrを入れておく
	IDXGIAdapter4* useAdapter_ = nullptr;

	ID3D12Device* device_ = nullptr;
};