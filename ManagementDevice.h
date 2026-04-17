#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include"ManagementLog.h"
#include<wrl.h>

class ManagementDevice {
	Microsoft::WRL::ComPtr<ID3D12Device>device_;
	
public:
	void CreateDevice(IDXGIAdapter4* useAdaptor);
	ID3D12Device* GetDevice()const { return device_.Get(); }

};