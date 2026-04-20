#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<wrl.h>

class ManagementCommand {
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

	float clearColor_[4] = { 0.1f,0.25f,0.5f,1.0f };
public:

	void Initialize(ID3D12Device* device);
	ID3D12CommandQueue* GetCommandQueue() const{ return commandQueue_.Get(); }
	ID3D12CommandAllocator* GetCommandAllocator()const { return commandAllocator_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); }
	void LoadCommand(IDXGISwapChain4* swapChain, D3D12_CPU_DESCRIPTOR_HANDLE*rtvHandles);
	void KickCommand(IDXGISwapChain4* swapChain);
};