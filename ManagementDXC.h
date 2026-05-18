#pragma once
#include <Windows.h>
#include<dxcapi.h>
#include<string>
#include <d3d12.h>
#include <dxgi1_6.h>

class ManagementDXC {
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	ID3DBlob* signatureBlob_ = nullptr;
	ID3DBlob* errorBlob_ = nullptr;
	ID3D12RootSignature* rootSignature_;
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[2] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	D3D12_BLEND_DESC blendDesc_{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicspipelineStateDesc{};
	ID3D12PipelineState* graphicsPipelineState = nullptr;
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

	/// <summary>
	/// 
	/// </summary>
	/// <param name="filePath">
	/// CompileするShaderファイルへのパス
	/// </param>
	/// <param name="profile">
	///　Compileに使用するProfile
	/// </param>
	/// <returns></returns>
	IDxcBlob* CompileShader(const std::wstring& filePath,const wchar_t* profile);
	void LoadingFile(const std::wstring& filePath, const wchar_t* profile, IDxcBlobEncoding*& shaderSource, DxcBuffer& shaderSourceBuffer);
	void Compiling(const std::wstring& filePath, const wchar_t* profile, DxcBuffer& shaderSourceBuffer, IDxcResult*& shaderResult);
	void CheckingError(IDxcResult*& shaderResult, IDxcBlobUtf8*& shaderError);
	void ReturnResult(const std::wstring& filePath, const wchar_t* profile, IDxcResult*& shaderResult, IDxcBlob*& shaderBlob, IDxcBlobEncoding*& shaderSource);
	void GenerateInstance();
	void SettingHandler();
	void GenerateRootSignature(ID3D12Device* device);
	void SettingInputLayout();
	void SettingBlendState();
	void SettingRasterizerState();
	void CompilingShader();
	void GeneratePSO(ID3D12Device* device);
	void SettingSampler();
public:
	void Initialize(ID3D12Device* device);
	ID3D12PipelineState* GetGraphicPipeLineState() { return graphicsPipelineState; }
	ID3D12RootSignature* GetRootSignature() { return rootSignature_; }
	void Release();
};