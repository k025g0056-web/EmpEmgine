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

	PipelineFactory::SetDefaultDesc(desc);
	PipelineFactory::GenerateRootSignature(object, desc, device);
	PipelineFactory::GeneratePSO(object, desc, shaderCompiler_, device);
}

void PipelineContainer::MakeParticleShaderPipeline(ID3D12Device* device) {

	PipelineDesc desc;
	desc.vsPath = L"Particle.VS.hlsl";
	desc.psPath = L"Particle.PS.hlsl";
	PipelineFactory::SetDefaultDesc(desc);
	desc.cullMode = D3D12_CULL_MODE_FRONT;
	desc.depth.writeMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	PipelineFactory::GenerateRootSignature(particle, desc, device);
	PipelineFactory::GeneratePSO(particle, desc, shaderCompiler_, device);
}