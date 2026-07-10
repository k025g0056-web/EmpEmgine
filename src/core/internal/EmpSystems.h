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

class EmpSystems {
public:
	static EmpSystems* GetInstance();
	void Initialize(int kWindowWidth, int kWindowHeight);
	int ProcessMessage();
	void SetWindowSize(unsigned int index, int windowWidth, int windowdHeight);
	void Finalize();
	void Begin();
	void Release();

	void DrawTriangle(const Vector3& v0, const Vector3& v1, 
		const Vector3& v2, const Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);

	void DrawTriangleColor(const Vector3& v0, const Vector3& v1,
		const Vector3& v2, const Vector4 color);
	void DrawTriangleColor(const Transform3d& transform, const Vector3& v0, const Vector3& v1,
		const Vector3& v2, const Vector4 color);

	void DrawTriangleTrans(const Transform3d& transform, const Vector3& v0,
		const Vector3& v1, const Vector3& v2, const Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);

	void DrawSprite(const Transform3d& transform, const Vector2& v0, 
		const Vector2& v1, const Vector2& v2, const Vector2& v3,const Vector4& color,
		D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle, const Transform3d& uvTransform);
	void DrawTextureSphere(const Transform3d& transform, const Vector4& color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);
	void DrawSphere(const Transform3d& transform, const Vector4& color);
	void DrawQuad(const Transform3d& transform, const Vector2& v0,
		const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector4& color);
	void DrawDoubleTriangle(const Transform3d& transform, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);
	void LightGUI();
	void PostDraw();
	void End();
	D3D12_GPU_DESCRIPTOR_HANDLE LoadTexture(const std::string& str);
	void SetWindowColor(Vector4 color);
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	void DrawPreModel(const Transform3d& transform, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle, const ModelData& modelData_);
	// 既存の DrawPreModel の下に追加
	void DrawCompressModel(
		const Transform3d& transform,
		D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle,
		Model& model);
	SoundData SoundLoadWave(const char* filename);
	void PlayAudio(const SoundData& soundData);
	void UnLoadAudio(SoundData* soundData);
	void DrawLight();

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
	ManagementTexture managementTexture_;
	MymGui mymGui_;
	D3D12_GPU_DESCRIPTOR_HANDLE white1x1{};
	DrawManager drawManager_;
	ManagementLighting managementLighting_;
	ManagementModel managementModel_;
	ManagementAudio managementAudio_;
	RenderTexture renderTexture_;
	int idx = 0;
	int douInd = 0;
	int sprInd = 0;
	int sphInd = 0;
	int modInd = 0;

	//------------------------------------------------------//
};
