#pragma once
#include<Windows.h>
#include"Internal/Window/managementWindow.h"
#include"Internal/DXGI/ManagementDXGIFactory.h"
#include"Internal/device/ManagementDevice.h"
#include"Internal/command/ManagementCommand.h"
#include"Internal/swapChain/ManagementSwapChain.h"
#include"Internal/descriptorHeap/ManagementDescriptorHeap.h"
#include"Internal/debug/ManagementDebug.h"
#include"Internal/DXC/ManagementDXC.h"
#include"Internal/viewPort/ManagementViewPort.h"
#include"appObj/Camera/Camera3d/Camera3d.h"
#include"Internal/ImGui/mine/MymGui.h"
#include"Internal/Loader/Texture/ManagementTexture.h"
#include<string>
#include"Interaction/Draw/Manager/DrawManager.h"
#include"DataModel/Vector2.h"
#include"appObj/Light/ManagementLighting.h"
#include"Internal/Loader/Model/ManagementModel.h"
#include"Internal/Loader/Audio/ManagementAudio.h"
#include"Internal/ImGui/Render/RenderTexture.h"
#include"Internal/Loader/Manager/LoaderManager.h"

class EmpSystems {
public:
	static EmpSystems* GetInstance();
	void Initialize(int kWindowWidth, int kWindowHeight);
	int ProcessMessage();
	void SetWindowSize(unsigned int index, int windowWidth, int windowdHeight);
	void Finalize();
	void Begin();
	void Release();
	void LightGUI();
	void PostDraw();
	void End();
	void SetWindowColor(Vector4 color);
	
	void DrawLight();

	LoaderManager& Resource() { return loader_; }
	DrawManager& Draw() { return drawManager_; }

	void GeneratePSO();
	void SetBlendMode(BlendMode blendMode);
	void SetRasterizer(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode);
	void SetDepthStencil(bool depthEnable, D3D12_DEPTH_WRITE_MASK DepthWriteMask, D3D12_COMPARISON_FUNC comparisonFunc);
	void SetPosition(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	void SetTexCoord(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	void SetNormal(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	void SetVertexShader(const std::wstring& filePath);
	void SetPixelShader(const std::wstring& filePath);
	ID3D12Device* GetDevice() { return managementDevice_.GetDevice(); }
	void SetPostDraw();

	D3D12_GPU_DESCRIPTOR_HANDLE GetWhite1x1() { return white1x1; }

private:
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
	MymGui mymGui_;
	D3D12_GPU_DESCRIPTOR_HANDLE white1x1{};
	DrawManager drawManager_;
	ManagementLighting managementLighting_;
	RenderTexture renderTexture_;
	LoaderManager loader_;
	

	//------------------------------------------------------//
};
