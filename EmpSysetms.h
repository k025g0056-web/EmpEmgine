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
#include"Camera3d.h"
#include"MymGui.h"
#include"ManagementTexture.h"
#include<string>

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
	ID3D12Resource* LoadTexture(const std::string& str);

	ManagementWindow managementWindow_;
	ManagementDXGIFactory managementDXGIFactory_;
	ManagementDevice managementDevice_;
	ManagementCommand managementCommand_;
	ManagementSwapChain managementSwapChain_;
	ManagementDescriptorHeap managementDescriptHeap_;
	ManagementDebug managementDebug_;
	ManagementDXC managementDXC_;
	ManagementViewPort managementViewPort_;
	ManagementTexture managementTexture_;
	Camera3d camera_;
	MymGui mymGui_;
	int windowWidth_;
	int windowHeight_;
	D3D12_GPU_DESCRIPTOR_HANDLE SRV;
};