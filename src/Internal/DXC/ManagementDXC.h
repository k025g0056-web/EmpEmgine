#pragma once
#include <Windows.h>
#include<dxcapi.h>
#include<string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include<wrl.h>

class ManagementDXC {
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;
	
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
	ID3D12PipelineState* GetGraphicPipeLineState() { return graphicsPipelineState_.Get(); }
	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }
	void Release();
};
