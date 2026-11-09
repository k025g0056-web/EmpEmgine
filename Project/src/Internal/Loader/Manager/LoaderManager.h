#pragma once
#include<Windows.h>
#include<string>
#include<d3d12.h>

#include"Internal/Loader/Texture/ManagementTexture.h"
#include"Internal/Loader/Model/ManagementModel.h"
#include"Internal/Loader/Audio/ManagementAudio.h"



class LoaderManager{
	ManagementModel managementModel_;
	ManagementAudio managementAudio_;
	ManagementTexture managementTexture_;
public:

	void Initialize(ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		ID3D12CommandQueue* commandQueue,
		ID3D12CommandAllocator* commandAllocator,
		HANDLE fenceEvent,
		uint64_t fenceValue,
		ID3D12Fence* fence,
		ID3D12DescriptorHeap* srvDescriptorHeap);
	ManagementTexture& Texture() { return managementTexture_; }
	ManagementAudio& Audio() { return managementAudio_; }
	ManagementModel& Model() { return managementModel_; }

	void Release();
};

