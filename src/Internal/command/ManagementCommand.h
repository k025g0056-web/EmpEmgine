#pragma once
#include <cstdint>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<wrl.h>
#include"DataModel/Vector4.h"

class ManagementCommand {
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};
	D3D12_RESOURCE_BARRIER barrier_{};
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_{};

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
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
	ID3D12CommandQueue* GetCommandQueue() const{ return commandQueue_.Get(); }
	ID3D12CommandAllocator* GetCommandAllocator()const { return commandAllocator_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); }
	HANDLE GetFenceEvent() { return fenceEvent_; }
	uint64_t& GetFenceValue() { return fenceValue_; }
	ID3D12Fence* GetFence() { return fence_.Get(); }
	void LoadCommand(IDXGISwapChain4* swapChain, D3D12_CPU_DESCRIPTOR_HANDLE*rtvHandles, ID3D12Resource** swapChainResources_,ID3D12DescriptorHeap* dsvDescriptorHeap);
	void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle);
	void KickCommand(IDXGISwapChain4* swapChain);
	void FenceRelease();
	void CommandRelease();
	
	void SetClearColor(Vector4 color) {
		clearColor_[0] = color.x;
		clearColor_[1] = color.y;
		clearColor_[2] = color.z;
		clearColor_[3] = color.w;
	}

	void WaitForGPU() {
		SendSignal();
		WaitingGPU();
	}

	void WaitCompleteCommand();
	void Excute();
	void Present(IDXGISwapChain4* swapChain);

	//細分化関数群
	void SetRSViewPort(D3D12_VIEWPORT viewPort);
	void SetRSScissorRects(D3D12_RECT scissorRect);
	void SetRootSignature(ID3D12RootSignature* rootSignature);
	void SetPipelineState(ID3D12PipelineState* graphicsPipelineState);
	void SetDescriptorHeaps(ID3D12DescriptorHeap* srvDescriptorHeap);

};
