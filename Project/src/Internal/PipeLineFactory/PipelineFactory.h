#pragma once
#include"DataModel/PipelineSet.h"
#include <Windows.h>
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include"internal/ShaderCompiler/ShaderCompiler.h"
#include"DataModel/BlendMode.h"

class PipelineFactory{
public:
	static void GenerateRootSignature(Pipeline& pipeline,const PipelineDesc& desc,ID3D12Device* device);
	static void GeneratePSO(Pipeline& pipeline, const PipelineDesc& desc,ShaderCompiler& compier, ID3D12Device* device);
	static D3D12_BLEND_DESC MakeBlendDesc(BlendMode blendMode);
};

