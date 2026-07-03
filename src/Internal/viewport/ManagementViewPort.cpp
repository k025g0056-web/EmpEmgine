#include"ManagementViewPort.h"
#include<cassert>
#include"Internal/DX12Mechanics/DX12Mechanics.h"


void ManagementViewPort::Initialize(ID3D12Device* device, int kWindowWidth, int kWindowHeight) {
	GenerateViewPort(kWindowWidth, kWindowHeight);
	CorrectionScissorRect(kWindowWidth, kWindowHeight);
	depthStencilResource_ = DX12Mechanics::CreateDepthStencilTextureResource(device, kWindowWidth, kWindowHeight);
}

//固有の関数
void ManagementViewPort::GenerateViewPort(int kWindowWidth, int kWindowHeight) {
	viewport.Width = static_cast<float>(kWindowWidth);
	viewport.Height = static_cast<float>(kWindowHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

//固有の関数
void ManagementViewPort::CorrectionScissorRect(int kWindowWidth, int kWindowHeight) {
	//基本的にビューポートと同じ形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = kWindowWidth;
	scissorRect.top = 0;
	scissorRect.bottom = kWindowHeight;
}

//解放
void ManagementViewPort::Release() {
	depthStencilResource_.Reset();
}

