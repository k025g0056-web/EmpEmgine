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
	ID3D12RootSignature* rootSignature_ = nullptr;
	ID3D12PipelineState* graphicsPipelineState = nullptr;
	
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
	void GeneratePSO(ID3D12Device* device);
	
public:
	void Initialize(ID3D12Device* device);
	ID3D12PipelineState* GetGraphicPipeLineState() { return graphicsPipelineState; }
	ID3D12RootSignature* GetRootSignature() { return rootSignature_; }
	void Release();
};
