#include "RenderTexture.h"

void RenderTexture::Initialize(ID3D12Device* device,
    int width,
    int height,
    ID3D12DescriptorHeap* srvHeap,
    UINT srvIndex) {

    D3D12_RESOURCE_DESC desc{};

    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Width = width;
    desc.Height = height;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

   

}

void RenderTexture::Begin(ID3D12GraphicsCommandList* cmd) {

}

void RenderTexture::End(ID3D12GraphicsCommandList* cmd) {

}

D3D12_GPU_DESCRIPTOR_HANDLE RenderTexture::GetGPUHandle() const {

}