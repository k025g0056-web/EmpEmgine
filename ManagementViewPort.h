#pragma once
#include<Windows.h>
#include <d3d12.h>
#include"Vector4.h"
#include"Matrix4x4.h"
#include"TransForm3d.h"
#include"Camera3d.h"
#include"VertexData.h"
#include<cstdint>
class ManagementViewPort {

	ID3D12Resource* depthStencilResource_ = nullptr;
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
	ID3D12Resource* GetDepthStencilResource() { return depthStencilResource_; }
	//----------------------------------------------------------------------------//
};