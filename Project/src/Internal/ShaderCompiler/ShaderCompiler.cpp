#include "ShaderCompiler.h"
#pragma comment(lib,"dxcompiler.lib")
#include"Log/ManagementLog.h"
#include<cassert>
#include<format>

void ShaderCompiler::Initialize() {
	GenerateInstance();
	SettingHandler();
}

Microsoft::WRL::ComPtr<IDxcBlob> ShaderCompiler::CompileShader(const std::wstring& filePath, const wchar_t* profile) {
	IDxcBlobEncoding* shaderSource = nullptr;
	DxcBuffer shaderSourceBuffer{};
	IDxcResult* shaderResult = nullptr;
	IDxcBlobUtf8* shaderError = nullptr;
	IDxcBlob* shaderBlob = nullptr;

	std::wstring file = L"resources/shader/" + filePath;

	//1.hlslファイルを読む
	LoadingFile(file, profile, shaderSource, shaderSourceBuffer);
	//2.Compileする
	Compiling(file, profile, shaderSourceBuffer, shaderResult);
	//3.警告・エラーが出てないか確認する
	CheckingError(shaderResult, shaderError);
	//4.Compile結果を受け取って返す
	ReturnResult(file, profile, shaderResult, shaderBlob, shaderSource);
	//実行用のバイナリを返却
	return shaderBlob;
}

void ShaderCompiler::LoadingFile(const std::wstring& filePath, const wchar_t* profile, IDxcBlobEncoding*& shaderSource, DxcBuffer& shaderSourceBuffer) {
	//これからシェーダーをコンパイルする旨をログに出す
	ManagementLog::Log(ManagementLog::ConvertToUTF8(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	HRESULT hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;
}

void ShaderCompiler::Compiling(const std::wstring& filePath, const wchar_t* profile, DxcBuffer& shaderSourceBuffer, IDxcResult*& shaderResult) {
	LPCWSTR arguments[] = {
		filePath.c_str(),//コンパイル対象のhlslファイル名
		L"-E",L"main",//エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile,//ShaderProfileの設定
		L"-Zi",L"-Qembed_debug",//デバッグ用の情報を埋め込む
		L"-Od",//最適化を外しておく
		L"-Zpr",//メモリレイアウトは行優先
	};

	//実際にShaderをコンパイルする
	HRESULT hr = dxcCompiler_->Compile(
		&shaderSourceBuffer,//読み込んだファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		includeHandler_.Get(),//インクルードが含まれた諸々
		IID_PPV_ARGS(&shaderResult)//コンパイル結果
	);

	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));
}

void ShaderCompiler::CheckingError(IDxcResult*& shaderResult, IDxcBlobUtf8*& shaderError) {
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		ManagementLog::Log(shaderError->GetStringPointer());
		assert(false);
	}
}

void ShaderCompiler::ReturnResult(const std::wstring& filePath, const wchar_t* profile, IDxcResult*& shaderResult, IDxcBlob*& shaderBlob, IDxcBlobEncoding*& shaderSource) {
	HRESULT hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したらログを出す
	ManagementLog::Log(ManagementLog::ConvertToUTF8(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
}

void ShaderCompiler::GenerateInstance() {
	HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));
}

void ShaderCompiler::SettingHandler() {
	HRESULT hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}
