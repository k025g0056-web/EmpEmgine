#pragma once
#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>
#include<filesystem>
#include<fstream>
#include<chrono>

class  ManagementLog{
	std::ofstream logStream_;
	void LogRock(const std::string& message);
public:
	void Initialize();
	static void Log(std::ofstream& os, const std::string& message);
	static void Log(const std::string& message);
	static void Log(const std::wstring& message);

};