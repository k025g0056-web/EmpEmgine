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
#include"DrawManager.h"
class EmpEngine;
class EmpSystems {

	friend class EmpEngine;
	void Initialize(int kWindowWidth, int kWindowHeight);
	int ProcessMessage();
	void SetWindowSize(unsigned int index, int windowWidth, int windowdHeight);
	void Finalize();
	void Begin();
	void Release();
	void DrawTriangleViewport();
	void DrawTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector4 color);
	void DrawTriangleTrans(const Transform3d& transform, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector4 color);
	void PostDraw();
	void End();
	void Update();
	ID3D12Resource* LoadTexture(const std::string& str);

	Transform3d transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
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
	DrawManager drawManager_;
};