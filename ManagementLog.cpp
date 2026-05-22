#include"ManagementLog.h"
#include <locale>
#include <codecvt>

std::ofstream ManagementLog::logStream_;
void ManagementLog::Initialize() {
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

	Log("Log start");
}

void ManagementLog::Log(std::ofstream& os, const std::string& message) {
	os << message << std::endl;
	OutputDebugStringA(message.c_str());
}

void ManagementLog::Log(const std::string& message) {
	logStream_ << message << std::endl;
	OutputDebugStringA(message.c_str());
}

void ManagementLog::Log(const std::wstring& message) {
	logStream_ << ConvertToUTF8(message) << '\n';
	OutputDebugStringW(message.c_str());
}

std::string ManagementLog::ConvertToUTF8(const std::wstring& wstr) {
	int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string result(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size, nullptr, nullptr);
	return result;
}

std::wstring ManagementLog::ConvertString(const std::string& str) {
	if (str.empty()) return L"";

	int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	if (size <= 0) return L"";

	std::wstring result(size - 1, L'\0');
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, result.data(), size);
	return result;
}