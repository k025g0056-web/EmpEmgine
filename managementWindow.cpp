#include"managementWindow.h"

void ManagementWindow::Initialize(int windowWidth,int windowHeight) {
	windows_.emplace_back();
	auto& window = windows_.back();
	wc_ = {};
	//ウィンドウプロージャ
	wc_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名（何でもいい）
	wc_.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wc_);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	window.wrc_ = {0,0,windowWidth,windowHeight};
	//クライアント領域を元に実際のサイズをwrcに変更してもらう
	AdjustWindowRect(&window.wrc_, WS_OVERLAPPEDWINDOW, false);

	window.hwnd_ = CreateWindow(
		wc_.lpszClassName,//利用するクラス名
		L"CG2",//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//よく見るウィンドウスタイル
		CW_USEDEFAULT,//表示X座標（Windowsに任せる）
		CW_USEDEFAULT,//表示Y座標（WindowsOSに任せる）
		window.wrc_.right - window.wrc_.left,//ウィンドウ横幅
		window.wrc_.bottom - window.wrc_.top,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		wc_.hInstance,//インスタンスハンドル
		this);//オプション

	//ウィンドウを表示する
	ShowWindow(window.hwnd_, SW_SHOW);
}

void ManagementWindow::GenerateWindow(int windowWidth, int windowHeight) {
	WindowData window{};

	window.wrc_ = { 0,0,windowWidth,windowHeight };
	AdjustWindowRect(&window.wrc_, WS_OVERLAPPEDWINDOW, false);

	window.hwnd_ = CreateWindow(
		wc_.lpszClassName,
		L"CG2",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		window.wrc_.right - window.wrc_.left,
		window.wrc_.bottom - window.wrc_.top,
		nullptr,
		nullptr,
		wc_.hInstance,
		this
	);
	
	ShowWindow(window.hwnd_, SW_SHOW);

	windows_.push_back(window);
}

void ManagementWindow::SetWindowSize(unsigned int index,int windowWidth,int windowHeight) {
	if (index>=windows_.size()) {
		return;
	}

	auto& window = windows_[index];

	RECT rc = { 0,0,windowWidth,windowHeight };
	AdjustWindowRect(&rc, WS_EX_OVERLAPPEDWINDOW, false);

	SetWindowPos(
		window.hwnd_,
		nullptr,
		0, 0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER);

	window.wrc_ = rc;

}

HWND ManagementWindow::GetHwnd(unsigned int index) {
	 if (index>=windows_.size()) {
		 return nullptr;
	 }

	 return windows_[index].hwnd_;
	 
}

RECT ManagementWindow::GetRect(unsigned int index) {
	if (index>=windows_.size()) {
		return RECT{};
	}

	return windows_[index].wrc_;
}

LRESULT CALLBACK ManagementWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
#ifdef USE_IMGUI

	if (ImGui_ImplWin32_WndProcHandler(hwnd,msg,wparam,lparam)) {
		return true;
	}

#endif // USE_IMGUI

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void ManagementWindow::Release() {
	for (auto& window:windows_) {
		CloseWindow(window.hwnd_);
	}

}