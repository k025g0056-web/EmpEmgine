#pragma once
#pragma once
#include <Windows.h>

#include <string>
#include <wrl.h>

#include <d3d12.h>
#include <dxgi1_6.h>

#include <dxcapi.h>
#include"Datamodel/BlendMode.h"

class ManagementDXC {
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;
	
	IDxcBlob* CompileShader(const std::wstring& filePath,const wchar_t* profile);
	void LoadingFile(const std::wstring& filePath, const wchar_t* profile, IDxcBlobEncoding*& shaderSource, DxcBuffer& shaderSourceBuffer);
	void Compiling(const std::wstring& filePath, const wchar_t* profile, DxcBuffer& shaderSourceBuffer, IDxcResult*& shaderResult);
	void CheckingError(IDxcResult*& shaderResult, IDxcBlobUtf8*& shaderError);
	void ReturnResult(const std::wstring& filePath, const wchar_t* profile, IDxcResult*& shaderResult, IDxcBlob*& shaderBlob, IDxcBlobEncoding*& shaderSource);
	void GenerateInstance();
	void SettingHandler();
	void GenerateRootSignature();

	D3D12_BLEND_DESC blendDesc_{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3] = {};
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device>device_;
	bool isDirty_ = true;

public:
	void Initialize(ID3D12Device* device);
	ID3D12PipelineState* GetGraphicPipeLineState() { return graphicsPipelineState_.Get(); }
	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }
	void Release();
	void GeneratePSO();
	void SetBlendMode(BlendMode blendMode);
	void SetRasterizer(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode);
	void SetDepthStencil(bool depthEnable, D3D12_DEPTH_WRITE_MASK DepthWriteMask, D3D12_COMPARISON_FUNC comparisonFunc);
	void SetPosition(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	void SetTexCoord(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	void SetNormal(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	void SetVertexShader(const std::wstring& filePath);
	void SetPixelShader(const std::wstring& filePath);
};
