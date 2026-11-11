#include "PipelineContainer.h"
#include"internal/PipelineFactory/PipelineFactory.h"

void PipelineContainer::Initialize(ID3D12Device* device) {
	shaderCompiler_.Initialize();
	MakeObjectShaderPipeline(device);
}

void PipelineContainer::MakeObjectShaderPipeline(ID3D12Device* device) {

	PipelineDesc desc;
	desc.rootParam = {
		{ RootParam::Type::CBV,      0, D3D12_SHADER_VISIBILITY_PIXEL },     // [0]
		{ RootParam::Type::CBV,      0, D3D12_SHADER_VISIBILITY_VERTEX },     // [1]
		{ RootParam::Type::SRVTABLE, 0, D3D12_SHADER_VISIBILITY_PIXEL, 1 },   // [2]
		{ RootParam::Type::CBV,      1, D3D12_SHADER_VISIBILITY_PIXEL },     // [3]
	};

	PipelineFactory::GenerateRootSignature(object, desc, device);
	PipelineFactory::GeneratePSO(object, desc, shaderCompiler_, device);
}