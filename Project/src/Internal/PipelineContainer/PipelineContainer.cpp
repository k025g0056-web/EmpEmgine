#include "PipelineContainer.h"
#include"internal/PipelineFactory/PipelineFactory.h"

void PipelineContainer::Initialize(ID3D12Device* device) {
	shaderCompiler_.Initialize();
	MakeObjectShaderPipeline(device);
	MakeParticleShaderPipeline(device);
}

void PipelineContainer::MakeObjectShaderPipeline(ID3D12Device* device) {

	PipelineDesc desc;
	desc.vsPath = L"Object3D.VS.hlsl";
	desc.psPath = L"Object3D.PS.hlsl";

	desc.rootParam = {
		{ RootParam::Type::CBV,      0, D3D12_SHADER_VISIBILITY_PIXEL },     // [0]
		{ RootParam::Type::CBV,      0, D3D12_SHADER_VISIBILITY_VERTEX },     // [1]
		{ RootParam::Type::SRVTABLE, 0, D3D12_SHADER_VISIBILITY_PIXEL, 1 },   // [2]
		{ RootParam::Type::CBV,      1, D3D12_SHADER_VISIBILITY_PIXEL },     // [3]
	};

	desc.sampler = { PipelineFactory::MakeDefaultSampler() };

	desc.inputElements = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT },
	};

	PipelineFactory::GenerateRootSignature(object, desc, device);
	PipelineFactory::GeneratePSO(object, desc, shaderCompiler_, device);
}

void PipelineContainer::MakeParticleShaderPipeline(ID3D12Device* device) {

	PipelineDesc desc;
	desc.vsPath = L"Particle.VS.hlsl";
	desc.psPath = L"Particle.PS.hlsl";

	desc.rootParam = {
		{ RootParam::Type::CBV,      0, D3D12_SHADER_VISIBILITY_PIXEL },     // [0]
		{ RootParam::Type::SRVTABLE, 0, D3D12_SHADER_VISIBILITY_VERTEX,1 },     // [1]
		{ RootParam::Type::SRVTABLE, 0, D3D12_SHADER_VISIBILITY_PIXEL, 1 },   // [2]
		{ RootParam::Type::CBV,      1, D3D12_SHADER_VISIBILITY_PIXEL },     // [3]
	};

	desc.sampler = { PipelineFactory::MakeDefaultSampler() };

	desc.cullMode = D3D12_CULL_MODE_NONE;

	desc.inputElements = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT },
	};

	PipelineFactory::GenerateRootSignature(particle, desc, device);
	PipelineFactory::GeneratePSO(particle, desc, shaderCompiler_, device);
}