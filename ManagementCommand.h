#pragma once
#include <cstdint>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<wrl.h>

class ManagementCommand {
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};
	D3D12_RESOURCE_BARRIER barrier_{};
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;

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
	void SettingDsvHandle(ID3D12DescriptorHeap* dsvDescriptorHeap, D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle, UINT backBufferIndex);
public:

	void Initialize(ID3D12Device* device);
	void PostDraw(D3D12_VIEWPORT viewPort,
		D3D12_RECT scissorRect,
		ID3D12PipelineState* graphicsPipelineState,
		ID3D12RootSignature* rootSignature, ID3D12DescriptorHeap* srvDescriptorHeap);
	ID3D12CommandQueue* GetCommandQueue() const{ return commandQueue_.Get(); }
	ID3D12CommandAllocator* GetCommandAllocator()const { return commandAllocator_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); }
	HANDLE GetFenceEvent() { return fenceEvent_; }
	uint64_t GetFenceValue() { return fenceValue_; }
	ID3D12Fence* GetFence() { return fence_; }
	void LoadCommand(IDXGISwapChain4* swapChain, D3D12_CPU_DESCRIPTOR_HANDLE*rtvHandles, ID3D12Resource** swapChainResources_,ID3D12DescriptorHeap* dsvDescriptorHeap);
	void KickCommand(IDXGISwapChain4* swapChain);
	void FenceRelease();
	void CommandRelease();
	void DrawCall(D3D12_VIEWPORT viewPort,
		D3D12_RECT scissorRect,D3D12_VERTEX_BUFFER_VIEW vertexBufferView, 
		ID3D12PipelineState* graphicsPipelineState, ID3D12RootSignature* rootSignature,
		ID3D12Resource* materialResource, ID3D12Resource* wvpResource,
		ID3D12DescriptorHeap* srvDescriptorHeap,
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);
};