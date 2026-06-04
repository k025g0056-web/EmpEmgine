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
#include"Vector2.h"
#include"ManagementLighting.h"

class EmpEngine;
class EmpSystems {

	friend class EmpEngine;
	void Initialize(int kWindowWidth, int kWindowHeight);
	int ProcessMessage();
	void SetWindowSize(unsigned int index, int windowWidth, int windowdHeight);
	void Finalize();
	void Begin();
	void Release();

	void DrawTriangle(const Vector3& v0, const Vector3& v1, 
		const Vector3& v2, const Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);

	void DrawTriangleTrans(const Transform3d& transform, const Vector3& v0,
		const Vector3& v1, const Vector3& v2, const Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);

	void DrawSprite(const Transform3d& transform, const Vector2& v0, 
		const Vector2& v1, const Vector2& v2, const Vector2& v3,const Vector4& color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);
	void DrawSphere(const Transform3d& transform, const Vector4& color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);
	void DrawColorSphere(const Transform3d& transform, const Vector4& color);
	void DrawQuad(const Transform3d& transform, const Vector2& v0,
		const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector4& color);
	void DrawDoubleTriangle(const Transform3d& transform, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);
	void LightGUI();
	void PostDraw();
	void End();
	D3D12_GPU_DESCRIPTOR_HANDLE LoadTexture(const std::string& str);


	//エンジンの変数
	//-----------------------------------------------------//
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
	MymGui mymGui_;
	D3D12_GPU_DESCRIPTOR_HANDLE white1x1{};
	DrawManager drawManager_;
	ManagementLighting managementLighting_;
	//------------------------------------------------------//
};
