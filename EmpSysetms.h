#pragma once
#include<Windows.h>
#include"managementWindow.h"
#include"ManagementDXGIFactory.h"
#include"ManagementDevice.h"
#include"ManagementCommand.h"
#include"ManagementSwapChain.h"
#include"ManagementDescriptorHeap.h"
#include"ManagementDebug.h"
#include"ManagementDXC.h"
#include"ManagementViewPort.h"
#include"Camera.h"
#include"MymGui.h"

class EmpEngine;
class EmpSystems {

	friend class EmpEngine;
	void Initialize(int kWindowWidth, int kWindowHeight);
	int ProcessMessage();
	void SetWindowSize(unsigned int index, int windowWidth, int windowdHeight);
	void Finalize();
	void Begin();
	void Release();
	void DrawTriangle();
	void End();
	void Update();

	ManagementWindow managementWindow_;
	ManagementDXGIFactory managementDXGIFactory_;
	ManagementDevice managementDevice_;
	ManagementCommand managementCommand_;
	ManagementSwapChain managementSwapChain_;
	ManagementDescriptorHeap managementDescriptHeap_;
	ManagementDebug managementDebug_;
	ManagementDXC managementDXC_;
	ManagementViewPort managementViewPort_;
	Camera camera_;
	MymGui mymGui_;
	int windowWidth_;
	int windowHeight_;
};