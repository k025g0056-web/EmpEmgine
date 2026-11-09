#pragma once
#include<Windows.h>
#include<strsafe.h>
#include<Dbghelp.h>
#pragma comment(lib,"Dbghelp.lib")


class CrashHandler {
public:
	static LONG WINAPI ExportDump(EXCEPTION_POINTERS* exception);
};