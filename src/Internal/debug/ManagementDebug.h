#pragma once
#include <Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<wrl.h>

class ManagementDebug {
	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController_;
public:
	void EnableDebugLayerWrapping();
	void ErrorDetection(ID3D12Device* device);
	void DebugReportLiveObject();
	void Release();
};