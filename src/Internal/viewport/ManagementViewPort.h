#pragma once
#include<Windows.h>
#include <d3d12.h>
#include"DataModel/Vector4.h"
#include"DataModel/Matrix4x4.h"
#include"DataModel/TransForm3d.h"
#include"appObj/Camera/Camera3d/Camera3d.h"
#include"DataModel/VertexData.h"
#include<cstdint>
#include<wrl.h>
class ManagementViewPort {

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};
	
	void GenerateViewPort(int kWindowWidth, int kWindowHeight);
	void CorrectionScissorRect(int kWindowWidth, int kWindowHeight);
public:
	void Initialize(ID3D12Device* device, int kWindowWidth, int kWindowHeight);
	void Release();

	//ゲッターロボ
	//-----------------------------------------------------------------------------//
	D3D12_VIEWPORT GetViewPort() { return viewport; }
	D3D12_RECT GetScissorRect() { return scissorRect; }
	ID3D12Resource* GetDepthStencilResource() { return depthStencilResource_.Get(); }
	//----------------------------------------------------------------------------//
};