#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <array>
#include <string>
#include <vector>
#include "BlendMode.h"

//ブレンドモードの要素数
inline constexpr size_t kBlendModeCount = static_cast<size_t>(kCountOfBlendMode);

struct RootParam{
	enum class Type{
		CBV,
		SRVTABLE
	};

	Type type;
	UINT shaderRegister;
	D3D12_SHADER_VISIBILITY visibility;
	UINT numDescriptor = 1;//タイプがSRVTABLEの時だけ使うニダ
};

struct InputElement {
	std::string semanticName;
	UINT semanticIndex;
	DXGI_FORMAT format;
	UINT offset = D3D12_APPEND_ALIGNED_ELEMENT;
};

struct DepthSetting{
	bool enable = true;
	D3D12_DEPTH_WRITE_MASK writeMask = D3D12_DEPTH_WRITE_MASK_ALL;
	D3D12_COMPARISON_FUNC func = D3D12_COMPARISON_FUNC_LESS_EQUAL;
};

struct RenderTargetFormat{
	DXGI_FORMAT rtv = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	DXGI_FORMAT dsv = DXGI_FORMAT_D24_UNORM_S8_UINT;
};

struct PipelineDesc{
	std::wstring vsPath;
	std::wstring psPath;
	std::vector<RootParam>rootParam;
	std::vector<D3D12_STATIC_SAMPLER_DESC>sampler;
	std::vector<InputElement>inputElements;
	D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK;
	D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	DepthSetting depth;
	RenderTargetFormat formats;
};

struct Pipeline{
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kBlendModeCount>psos;

	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	ID3D12PipelineState* GetPSO(BlendMode blendMode) { return psos[blendMode].Get(); }

	void Reset() {
		rootSignature.Reset();
		for (auto& p : psos) { p.Reset(); }
	}
};

