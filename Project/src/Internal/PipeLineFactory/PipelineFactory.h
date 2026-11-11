#pragma once
#include"DataModel/PipelineSet.h"
#include <Windows.h>
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

class PipelineFactory{
public:
	static void GenerateRootSignature(Pipeline& pipeline,PipelineDesc& desc,ID3D12Device* device);



};

