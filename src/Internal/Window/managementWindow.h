#pragma once
#include<Windows.h>
#include<vector>
#ifdef USE_IMGUI
#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wPram, LPARAM lParam);
#endif // USE_IMGUI
class ManagementWindow {
	struct WindowData {
		RECT wrc_;
		HWND hwnd_;
	};

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	WNDCLASS wc_{};
	std::vector<WindowData> windows_{};
	
	UINT pendingWidth_ = 0;
	UINT pendingHeight_ = 0;
	bool resizePending_ = false;

public:
	void Initialize(int kWindowWidth, int kWindowHeight);
	void GenerateWindow(int kWindowWidth, int kWindowHeight);
	void SetWindowSize(unsigned int index, int windowWidth, int windowHeight);

	void Release();
	HWND GetHwnd(unsigned int i);
	RECT GetRect(unsigned int i);
	bool& GetResizePending() { return resizePending_; }
	UINT GetPendingWidth() { return pendingWidth_; }
	UINT GetPendingHeight() { return pendingHeight_; }
	void SetResizePending(bool resizePending) { resizePending_ = resizePending; }
	void SetWindowPendingWidth(UINT width) { pendingWidth_ = width; }
	void SetWindowPendingHeight(UINT height) { pendingHeight_ = height; }
};