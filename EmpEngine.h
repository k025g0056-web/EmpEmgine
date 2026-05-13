#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include"managementWindow.h"
#include"ManagementLog.h"
#include"ManagementDXGIFactory.h"
#include"ManagementDevice.h"
#include"ManagementCommand.h"
#include"ManagementSwapChain.h"
#include"ManagementDescriptorHeap.h"
#include"ManagementDebug.h"
#include"ManagementDXC.h"
#include"ManagementViewPort.h"
#include"Camera.h"

class EmpEngine {
public:
	static void Initialize(int kWindowWidth, int kWindowHeight);
	static int ProcessMessage();
	static void SetWindowSize(unsigned int index, int windowWidth, int windowHeight);
	static void Finalize();
	static void Begin();
	static void DrawTriangle();
	static void End();
	static void Update(int kWindowWidth, int kWindowHeight);
private:
	static EmpEngine& GetInstance();
	void InitializeImpl(int kWindowWidth, int kWindowHeight);
	int ProcessMessageImpl();
	void SetWindowSizeImpl(unsigned int index, int windowWidth, int windowdHeight);
	void FinalizeImpl();
	void BeginImpl();
	void Release();
	void DrawTriangleImpl();
	void EndImpl();
	void UpdateImpl(int kWindowWidth, int kWindowHeight);
private:

	ManagementWindow managementWindow_;
	ManagementDXGIFactory managementDXGIFactory_;
	ManagementDevice managementDevice_;
	ManagementCommand managementCommand_;
	ManagementSwapChain managementSwapChain_;
	ManagementDescriptorHeap managementDescriptHeap_;
	ManagementDebug managementDebug_;
	ManagementDXC managementDXC_;
	ManagementViewPort managementViewPort_;
	Camera camera;
};