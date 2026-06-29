#include "RenderTexture.h"

void RenderTexture::Initialize(
    ID3D12Device* device,
    UINT width,
    UINT height,

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle,

    D3D12_CPU_DESCRIPTOR_HANDLE srvCpu,

    D3D12_GPU_DESCRIPTOR_HANDLE srvGpu)
{
    rtv_ = rtvHandle;
    srvCPU_ = srvCpu;
    srvGPU_ = srvGpu;

    D3D12_RESOURCE_DESC desc{};

    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Width = width;
    desc.Height = height;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    D3D12_HEAP_PROPERTIES heap{};

    heap.Type = D3D12_HEAP_TYPE_DEFAULT;

    FLOAT clearColor[4] =
    {
        0.3f,
        0.3f,
        0.3f,
        1.0f
    };

    D3D12_CLEAR_VALUE clear{};

    clear.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    memcpy(clear.Color, clearColor, sizeof(clearColor));

    device->CreateCommittedResource(
        &heap,

        D3D12_HEAP_FLAG_NONE,

        &desc,

        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,

        &clear,

        IID_PPV_ARGS(texture_.GetAddressOf()));



    device->CreateRenderTargetView(
        texture_.Get(),

        nullptr,

        rtv_);




    D3D12_SHADER_RESOURCE_VIEW_DESC srv{};

    srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

    srv.Texture2D.MipLevels = 1;

    srv.Shader4ComponentMapping =
        D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    device->CreateShaderResourceView(
        texture_.Get(),

        &srv,

        srvCPU_);
}

void RenderTexture::Begin(
    ID3D12GraphicsCommandList* cmd,
    D3D12_CPU_DESCRIPTOR_HANDLE dsv)
{
    D3D12_RESOURCE_BARRIER barrier{};

    barrier.Type =
        D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

    barrier.Transition.pResource =
        texture_.Get();

    barrier.Transition.StateBefore =
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

    barrier.Transition.StateAfter =
        D3D12_RESOURCE_STATE_RENDER_TARGET;

    cmd->ResourceBarrier(1, &barrier);

    cmd->OMSetRenderTargets(
        1,

        &rtv_,

        FALSE,

        &dsv);

    FLOAT clear[4] =
    {
        0.2f,
        0.2f,
        0.2f,
        1.0f
    };

    cmd->ClearRenderTargetView(
        rtv_,

        clear,

        0,

        nullptr);
}

void RenderTexture::End(
    ID3D12GraphicsCommandList* cmd)
{
    D3D12_RESOURCE_BARRIER barrier{};

    barrier.Type =
        D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

    barrier.Transition.pResource =
        texture_.Get();

    barrier.Transition.StateBefore =
        D3D12_RESOURCE_STATE_RENDER_TARGET;

    barrier.Transition.StateAfter =
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

    cmd->ResourceBarrier(1, &barrier);
}