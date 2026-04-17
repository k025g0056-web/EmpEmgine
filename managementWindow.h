#pragma once
#include<Windows.h>
#include<vector>

class ManagementWindow {
	struct WindowData {
		RECT wrc_;
		HWND hwnd_;
	};

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	WNDCLASS wc_;
	std::vector<WindowData> windows_;


public:
	void Initialize(int kWindowWidth, int kWindowHeight);
	void GenerateWindow(int kWindowWidth, int kWindowHeight);
	void SetWindowSize(int index, int windowWidth, int windowHeight);

	HWND GetHwnd(int i);
	RECT GetRect(int i);
};