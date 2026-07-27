#include "LoaderManager.h"

void LoaderManager::Initialize(ID3D12Device* device,
	ID3D12GraphicsCommandList* commandList,
	ID3D12CommandQueue* commandQueue,
	ID3D12CommandAllocator* commandAllocator,
	HANDLE fenceEvent,
	uint64_t fenceValue,
	ID3D12Fence* fence,
	ID3D12DescriptorHeap* srvDescriptorHeap) {
	managementTexture_.Initialize(device, commandList, commandQueue, commandAllocator, fenceEvent, fenceValue, fence, srvDescriptorHeap);
	managementAudio_.Initialize();
}

void LoaderManager::Release() {
	managementTexture_.Release();
	managementAudio_.Release();
}

