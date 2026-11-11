#pragma once
#include <Windows.h>
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include"Datamodel/BlendMode.h"
#include"Datamodel/DrawMode.h"
#include"Datamodel/FillMode.h"

class ManagementDXC {
	struct GraphicsPipeline {
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	};

	struct PipelineDesc{
		std::wstring vsPath;
		std::wstring psPath;
		BlendMode blendMode = kBlendModeNone;
		D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_NONE;
		D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID;
		bool depthEnable = true;
		D3D12_DEPTH_WRITE_MASK deothWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		D3D12_COMPARISON_FUNC depthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	};

	GraphicsPipeline objectPipeline_;
	GraphicsPipeline particlePipeline_;

	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_;
	
	IDxcBlob* CompileShader(const std::wstring& filePath,const wchar_t* profile);
	void LoadingFile(const std::wstring& filePath, const wchar_t* profile, IDxcBlobEncoding*& shaderSource, DxcBuffer& shaderSourceBuffer);
	void Compiling(const std::wstring& filePath, const wchar_t* profile, DxcBuffer& shaderSourceBuffer, IDxcResult*& shaderResult);
	void CheckingError(IDxcResult*& shaderResult, IDxcBlobUtf8*& shaderError);
	void ReturnResult(const std::wstring& filePath, const wchar_t* profile, IDxcResult*& shaderResult, IDxcBlob*& shaderBlob, IDxcBlobEncoding*& shaderSource);
	void GenerateInstance();
	void SettingHandler();
	void GenerateRootSignature(GraphicsPipeline& pipeline);

	D3D12_BLEND_DESC blendDesc_{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3] = {};
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device>device_;
	bool isDirty_ = true;

	void Reset(GraphicsPipeline& pipeLine);

	void GenerateObjectPipeLine();
	void GenerateParticlePipeLine();


	void CreatePipeline(GraphicsPipeline& pipeline, const PipelineDesc& desc);
	static D3D12_BLEND_DESC MakeBlendDesc(BlendMode blendMode);

public:
	void Initialize(ID3D12Device* device);
	ID3D12PipelineState* GetOBJGraphicPipeLineState() { return objectPipeline_.pipelineState.Get(); }
	ID3D12RootSignature* GetOBJRootSignature() { return objectPipeline_.rootSignature.Get(); }
	void Release();
	void GeneratePSO(GraphicsPipeline& pipeline);
	void SetBlendMode(BlendMode blendMode);
	void SetRasterizer(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode);
	void SetDepthStencil(bool depthEnable, D3D12_DEPTH_WRITE_MASK DepthWriteMask, D3D12_COMPARISON_FUNC comparisonFunc);
	void SetPosition(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	void SetTexCoord(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	void SetNormal(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	void SetVertexShader(const std::wstring& filePath);
	void SetPixelShader(const std::wstring& filePath);
};
