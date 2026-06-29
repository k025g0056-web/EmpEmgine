#pragma once
#include <d3d12.h>
#include<wrl.h>

class RenderTexture{

public:
    void Initialize(ID3D12Device* device,
        int width,
        int height,
        ID3D12DescriptorHeap* srvHeap,
        UINT srvIndex);
    void Begin(ID3D12GraphicsCommandList* cmd);
    void End(ID3D12GraphicsCommandList* cmd);

    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const;

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> texture_;

    D3D12_CPU_DESCRIPTOR_HANDLE rtv_;
    D3D12_CPU_DESCRIPTOR_HANDLE srvCPU_;
    D3D12_GPU_DESCRIPTOR_HANDLE srvGPU_;


};

