#pragma once
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <wrl.h>
#include"Log/ManagementLog.h"

class ManagementDXGIFactory {
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<IDXGIAdapter4>  useAdapter_;

public:
	void Initialize();
	void DecideAdapter();

	IDXGIFactory7* GetDXGIFactory() const{ return dxgiFactory_.Get(); }
	IDXGIAdapter4* GetUseAdapter() const{ return useAdapter_.Get(); }

	void Release();
};