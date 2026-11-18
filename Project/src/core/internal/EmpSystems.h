#pragma once
#include<Windows.h>
#include"Internal/Window/managementWindow.h"
#include"Internal/DXGI/ManagementDXGIFactory.h"
#include"Internal/device/ManagementDevice.h"
#include"Internal/command/ManagementCommand.h"
#include"Internal/swapChain/ManagementSwapChain.h"
#include"Internal/descriptorHeap/ManagementDescriptorHeap.h"
#include"Internal/debug/ManagementDebug.h"
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
#include"Internal/FrameRate/ManagementFrameRate.h"
#include"internal/PipelineContainer/PipelineContainer.h"
#include<random>

class EmpSystems {
	void ResizeWindow();
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
	ID3D12Device* GetDevice() { return managementDevice_.GetDevice(); }
	float DeltaTime() { return framerate_.GetDeltaTime(); }
	int GetWhite1x1() { return white1x1; }
	void RebindRenderTarget();
	std::mt19937& GetRandom() { return randomEngine_; }
	SrvSlot GetHeapId() { return managementDescriptHeap_.AllocateSrv(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUPtr(UINT index) { return managementDescriptHeap_.GetCPUPtr(index); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUPtr(UINT index) { return managementDescriptHeap_.GetGPUPtr(index); }

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
	ManagementViewPort managementViewPort_;
	MymGui mymGui_;
	int white1x1{};
	DrawManager drawManager_;
	ManagementLighting managementLighting_;
	RenderTexture renderTexture_;
	LoaderManager loader_;
	ManagementFrameRate framerate_;
	PipelineContainer pipelinecontainer_;
	std::mt19937 randomEngine_;
	//------------------------------------------------------//
};
