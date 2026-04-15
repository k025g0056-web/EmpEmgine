#include"EmpEngine.h"

EmpEngine* EmpEngine::instance_ = nullptr;
void EmpEngine::Initialize() {
	//誰も捕捉しなかった場合に(Unhandled)、捕捉する関数を登録
	//main関数が始まってすぐに登録すると良い
	SetUnhandledExceptionFilter(ExportDump);
	instance_ = new EmpEngine();
	instance_->WindowInitialize();
	instance_->LogInitialize();
	instance_->DXGIInitialize();
	instance_->DecideAdapter();
	instance_->GenerateDevice();
	instance_->GenerateCommandQueue();
	instance_->GenerateCommandList();
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
	std::string logFilePath = std::string("logs/") + dateString + "log";
	//ファイルを作って書き込み準備
	std::ofstream logStream(logFilePath);

	Log(logStream, "Log start");
}

void EmpEngine::DXGIInitialize() {
	hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	//初期化の根本的な部分でエラーが出た場合はプログラムが間違っているか、
	// どうにも出来ない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr_));
}

void EmpEngine::DecideAdapter() {
	//良い順にアダプタを組む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) !=
		DXGI_ERROR_NOT_FOUND; ++i) {
		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr_ = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr_));//取得できないのは一大事
		//ソフトウェアアダプタでなければ採用‼
		if (!(adapterDesc.Flags&DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用したアダプタの情報をログに出力。wstringの方なので注意
			Log(std::format(L"USE Adapter:{}\n", adapterDesc.Description));
			break;
		}

		useAdapter_ = nullptr;//ソフトウェアの場合は見なかったことにする
	}

	//適切なアダプタがみつからなかったので起動できない
	assert(useAdapter_ != nullptr);
}

void EmpEngine::GenerateDevice() {
	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};

	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプターでデバイスを生成
		hr_ = D3D12CreateDevice(useAdapter_, featureLevels[i], IID_PPV_ARGS(&device_));
		//指定した機能レベルでデバイスが生成出来たかを確認
		if (SUCCEEDED(hr_)) {
			//生成出来たのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel:{}\n", featureLevelStrings[i]));
			break;
		}
	}

	//デバイスの生成が上手く行かなかったので起動できない
	assert(device_ != nullptr);
	Log("Complete create D3D12Device!!!\n");
}

LONG WINAPI EmpEngine::ExportDump(EXCEPTION_POINTERS* exception) {
	//時刻を取得して、時刻を名前に入れたファイル
	SYSTEMTIME time;
	GetLocalTime(&time);
	wchar_t filePath[MAX_PATH] = { 0 };
	CreateDirectory(L"./Dumps", nullptr);
	StringCchPrintfW(filePath,MAX_PATH ,L"./Dumps/%04d-%02d%02d-%02d%02d.dmp", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
	HANDLE dumpFileHandle = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	//processId(このexeのId)とクラッシュ(例外)の発生したthreadIdを取得
	DWORD processId = GetCurrentProcessId();
	DWORD threadId = GetCurrentThreadId();
	//設定情報を入力
	_MINIDUMP_EXCEPTION_INFORMATION minidumpInformation{ 0 };
	minidumpInformation.ThreadId = threadId;
	minidumpInformation.ExceptionPointers = exception;
	minidumpInformation.ClientPointers = TRUE;
	//Dumpを出力。MiniDumpNormalは最低限の情報を出力するフラグ
	MiniDumpWriteDump(GetCurrentProcess(), processId, dumpFileHandle, MiniDumpNormal, &minidumpInformation, nullptr, nullptr);
	//他に関連づけられているSEH例外ハンドラがあれば実行。通常はプロセスを終了する
	return EXCEPTION_EXECUTE_HANDLER;
}

void EmpEngine::GenerateCommandQueue() {
	//コマンドキューの生成
	commandQueueDesc_={};
	hr_ = device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
	//コマンドキューの生成が上手く行かなかったので実行出来ない
	assert(SUCCEEDED(hr_));
}

void EmpEngine::GenerateCommandList() {
	hr_ = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	//コマンドアロケーターの生成がうまくいかなかったので生成できない
	assert(SUCCEEDED(hr_));

	hr_ = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_, nullptr, IID_PPV_ARGS(&commandList_));
	//コマンドリストの生成が上手く行かなかったので起動できない
	assert(SUCCEEDED(hr_));
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

void CALLBACK EmpEngine::DebugPrint(LPCSTR lpOutputStrings) {
	OutputDebugStringA(lpOutputStrings);
}

void EmpEngine::Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

void EmpEngine::Log(std::ofstream& os, const std::string& message) {
	os << message << std::endl;
	OutputDebugStringA(message.c_str());
}

void EmpEngine::Log(const std::wstring& message) {
	OutputDebugStringW(message.c_str());
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