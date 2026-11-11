#include "PipelineFactory.h"
#pragma comment(lib,"dxcompiler.lib")
#include<cassert>
#include"Log/ManagementLog.h"
#include<format>

void PipelineFactory::GenerateRootSignature(Pipeline& pipeline, const PipelineDesc& desc,ID3D12Device* device) {
	std::vector<D3D12_ROOT_PARAMETER> rootParameters(desc.rootParam.size());

	std::vector<D3D12_DESCRIPTOR_RANGE> ranges;
	ranges.reserve(desc.rootParam.size());

	for (size_t i = 0; i < desc.rootParam.size();++i) {
		const RootParam& src = desc.rootParam[i];
		D3D12_ROOT_PARAMETER& dst = rootParameters[i];
		dst.ShaderVisibility = src.visibility;

		switch (src.type){
		case RootParam::Type::CBV:
			dst.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
			dst.Descriptor.ShaderRegister = src.shaderRegister;
			break;
		case RootParam::Type::SRVTABLE:
		{
			D3D12_DESCRIPTOR_RANGE range{};
			range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			range.NumDescriptors = src.numDescriptor;
			range.BaseShaderRegister = src.shaderRegister;
			range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
			ranges.push_back(range);

			dst.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			dst.DescriptorTable.NumDescriptorRanges = 1;
			dst.DescriptorTable.pDescriptorRanges = &ranges.back();
			break;
		}
		}
	}

	D3D12_ROOT_SIGNATURE_DESC rsDesc{};
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rsDesc.pParameters = rootParameters.data();
	rsDesc.NumStaticSamplers = static_cast<UINT>(rootParameters.size());
	rsDesc.pStaticSamplers = desc.sampler.data();
	rsDesc.NumStaticSamplers = static_cast<UINT>(desc.sampler.size());

	Microsoft::WRL::ComPtr<ID3DBlob>signatureBlob;
	Microsoft::WRL::ComPtr<ID3D10Blob> errorBlob;
	HRESULT hr = D3D12SerializeRootSignature(&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		if (errorBlob) {
			ManagementLog::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}

		assert(false);
	}

	hr = device->CreateRootSignature(0,
		signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipeline.rootSignature));
	assert(SUCCEEDED(hr));
}
