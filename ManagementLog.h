#pragma once
#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>
#include<filesystem>
#include<fstream>
#include<chrono>

class  ManagementLog{
	static std::ofstream logStream_;
public:
	static void Initialize();
	static void Log(std::ofstream& os, const std::string& message);
	static void Log(const std::string& message);
	static void Log(const std::wstring& message);
	static std::string ConvertToUTF8(const std::wstring& wstr);
};