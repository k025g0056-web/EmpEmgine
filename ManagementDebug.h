#pragma once
#include <Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>

class ManagementDebug {
	ID3D12Debug1* debugController_ = nullptr;

public:
	void EnableDebugLayerWrapping();
	void ErrorDetection(ID3D12Device* device);
};