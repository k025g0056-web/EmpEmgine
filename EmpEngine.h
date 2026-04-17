#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include"managementWindow.h"
#include"ManagementLog.h"
#include"ManagementDXGIFactory.h"
#include"ManagementDevice.h"
#include"ManagementCommand.h"

class EmpEngine {
public:
	static void Initialize(int kWindowWidth, int kWindowHeight);
	static int ProcessMessage();
	static void SetWindowSize(int index, int windowWidth, int windowHeight);
	static void Finalize();

private:
	static EmpEngine& GetInstance();
	void InitializeImpl(int kWindowWidth, int kWindowHeight);
	int ProcessMessageImpl();
	void SetWindowSizeImpl(int index, int windowWidth, int windowdHeight);
	void FinalizeImpl();

private:
	
	//初期化用のインスタンス
	static EmpEngine* instance_;

	ManagementWindow managementWindow_;
	ManagementLog managementLog_;
	ManagementDXGIFactory managementDXGIFactory_;
	ManagementDevice managementDevice_;
	ManagementCommand managementCommand_;
};