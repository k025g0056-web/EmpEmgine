#include"EmpEngine.h"

EmpEngine* EmpEngine::instance_ = nullptr;
void EmpEngine::Initialize() {
	instance_ = new EmpEngine();
	instance_->WindowInitialize();
	instance_->LogInitialize();
}

void EmpEngine::WindowInitialize() {
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
	wrc_ = { 0,0,kClientWidth,kClientHeight };
	//クライアント領域を元に実際のサイズをwrcに変更してもらう
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);

	hwnd_ = CreateWindow(
		wc_.lpszClassName,//利用するクラス名
		L"CG2",//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//よく見るウィンドウスタイル
		CW_USEDEFAULT,//表示X座標（Windowsに任せる）
		CW_USEDEFAULT,//表示Y座標（WindowsOSに任せる）
		wrc_.right - wrc_.left,//ウィンドウ横幅
		wrc_.bottom - wrc_.top,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		wc_.hInstance,//インスタンスハンドル
		this);//オプション

	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

void EmpEngine::LogInitialize() {
	//もしディレクトリがないのであれば作成する
	if (!std::filesystem::exists("logs")) {
		std::filesystem::create_directory("logs");
	}

	//現在時刻を取得(UTC時刻)
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	//ログファイルの名前にコンマ何秒はいらないので、削って秒にする
	std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>
		nowSeconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	//日本時間(PCの設定時間)に変換
	std::chrono::zoned_time localTime{ std::chrono::current_zone(),nowSeconds };
	//formatを使って年月日_時分秒の文字列に変換
	std::string dateString = std::format("{:%Y%m%d_%H%M%S}", localTime);
	//時刻を使ってファイル名を決定
	std::string logFilePath = std::string("logs/") + dateString + ".log";
	//ファイルを作って書き込み準備
	logStream_.open(logFilePath);

	Log(logStream_, "Log start");
}

int EmpEngine::ProcessMessage() {
	MSG msg{};
	//Windowにメッセージが来ていたら最優先で処理させる
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message==WM_QUIT) {
			return 1;
		}
	}

	return 0;
}

void __stdcall EmpEngine::DebugPrint(LPCSTR lpOutputStrings) {
	OutputDebugStringA(lpOutputStrings);
}

void EmpEngine::Log(const std::string& message) {
	instance_->logStream_ << message << std::endl;
	OutputDebugStringA(message.c_str());
}

void EmpEngine::Log(std::ofstream& os, const std::string& message) {
	os << message << std::endl;
	OutputDebugStringA(message.c_str());
}

LRESULT CALLBACK EmpEngine::WindowProc(HWND hwnd,UINT msg,WPARAM wparam, LPARAM lparam) {
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg){
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void EmpEngine::Finalize() {
	delete instance_;
	instance_ = nullptr;
}