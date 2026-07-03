#pragma once
#include <d3d12.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class RenderTexture
{
public:

    void Initialize(
        ID3D12Device* device,
        UINT width,
        UINT height,

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle,

        D3D12_CPU_DESCRIPTOR_HANDLE srvCpu,

        D3D12_GPU_DESCRIPTOR_HANDLE srvGpu);

    void Begin(
        ID3D12GraphicsCommandList* cmd,
        D3D12_CPU_DESCRIPTOR_HANDLE dsv);

    void End(
        ID3D12GraphicsCommandList* cmd);

    D3D12_GPU_DESCRIPTOR_HANDLE GetSRV() const
    {
        return srvGPU_;
    }

private:

    ComPtr<ID3D12Resource> texture_;

    D3D12_CPU_DESCRIPTOR_HANDLE rtv_{};

    D3D12_CPU_DESCRIPTOR_HANDLE srvCPU_{};

    D3D12_GPU_DESCRIPTOR_HANDLE srvGPU_{};
};