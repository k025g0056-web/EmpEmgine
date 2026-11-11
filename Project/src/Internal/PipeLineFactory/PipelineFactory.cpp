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

void PipelineFactory::GeneratePSO(Pipeline& pipeline, const PipelineDesc& desc, ShaderCompiler& compier, ID3D12Device* device) {
	assert(pipeline.rootSignature && "ルートシグネチャないね関数の順番を疑って見よう");

	auto vs = compier.CompileShader(desc.vsPath, L"vs_6_0");
	auto ps = compier.CompileShader(desc.psPath, L"vs_6_0");
	assert(vs && "バーテックスシェーダーがないねぇ");
	assert(ps && "ピクセルシェーダーがないねぇ");

	std::vector<D3D12_INPUT_ELEMENT_DESC> elements;
	for (const InputElement& e:desc.inputElements) {
		D3D12_INPUT_ELEMENT_DESC d{};
		d.SemanticName = e.semanticName.c_str();
		d.SemanticIndex = e.semanticIndex;
	}
}

D3D12_BLEND_DESC PipelineFactory::MakeBlendDesc(BlendMode blendMode) {
	D3D12_BLEND_DESC blendDesc;

	//ブレンドの状態の設定
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	switch (blendMode) {
	case kBlendModeNone:
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		break;
	case kBlendModeNormal:
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		break;
	case kBlendModeAdd:
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

		break;
	case kBlendModeSubtract:
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;

		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

		break;
	case kBlendModeMultiply:
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;

		break;
	case kBlendModeScreen:
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

		break;
	case kCountOfBlendMode:
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		break;
	default:
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		break;
	}

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	return blendDesc;
}
