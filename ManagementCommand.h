#pragma once
#include <cstdint>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<wrl.h>

class ManagementCommand {
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};
	D3D12_RESOURCE_BARRIER barrier_{};

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	ID3D12Fence* fence_ = nullptr;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_ = nullptr;


	float clearColor_[4] = { 0.1f,0.25f,0.5f,1.0f };

	void GenerateCommandQueue(ID3D12Device* device);
	void GenerateCommandAllocator(ID3D12Device* device);
	void GenerateCommandList(ID3D12Device* device);
	void PutUpABarrier(ID3D12Resource* swapChainResources_[], unsigned int index);
	void PutUpReBarrier();
	void GenerateFence(ID3D12Device* device);
	void SendSignal();
	void WaitingGPU();
public:

	void Initialize(ID3D12Device* device);
	ID3D12CommandQueue* GetCommandQueue() const{ return commandQueue_.Get(); }
	ID3D12CommandAllocator* GetCommandAllocator()const { return commandAllocator_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); }
	void LoadCommand(IDXGISwapChain4* swapChain, D3D12_CPU_DESCRIPTOR_HANDLE*rtvHandles, ID3D12Resource** swapChainResources_);
	void KickCommand(IDXGISwapChain4* swapChain);
};