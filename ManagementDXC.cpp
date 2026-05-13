#include"ManagementDXC.h"
#pragma comment(lib,"dxcompiler.lib")
#include<cassert>
#include"ManagementLog.h"
#include<format>

void ManagementDXC::Initialize(ID3D12Device* device) {
	GenerateInstance();
	SettingHandler();
	GenerateRootSignature(device);
	SettingInputLayout();
	SettingBlendState();
	SettingRasterizerState();
	CompilingShader();
	GeneratePSO(device);
}

void ManagementDXC::GenerateInstance() {
	HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));
}

void ManagementDXC::SettingHandler() {
	HRESULT hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

IDxcBlob* ManagementDXC::CompileShader(const std::wstring& filePath, const wchar_t* profile) {
	IDxcBlobEncoding* shaderSource = nullptr;
	DxcBuffer shaderSourceBuffer{};
	IDxcResult* shaderResult = nullptr;
	IDxcBlobUtf8* shaderError = nullptr;
	IDxcBlob* shaderBlob = nullptr;

	//1.hlslファイルを読む
	LoadingFile(filePath, profile,shaderSource,shaderSourceBuffer);
	//2.Compileする
	Compiling(filePath, profile,shaderSourceBuffer,shaderResult);
	//3.警告・エラーが出てないか確認する
	CheckingError(shaderResult,shaderError);
	//4.Compile結果を受け取って返す
	ReturnResult(filePath,profile,shaderResult,shaderBlob,shaderSource);
	//実行用のバイナリを返却
	return shaderBlob;
}

void ManagementDXC::LoadingFile(const std::wstring& filePath, const wchar_t* profile, IDxcBlobEncoding*& shaderSource, DxcBuffer& shaderSourceBuffer) {
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

void ManagementDXC::Compiling(const std::wstring& filePath, const wchar_t* profile, DxcBuffer& shaderSourceBuffer, IDxcResult*& shaderResult) {
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
		includeHandler_,//インクルードが含まれた諸々
		IID_PPV_ARGS(&shaderResult)//コンパイル結果
	);

	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));
}

void ManagementDXC::CheckingError(IDxcResult*& shaderResult, IDxcBlobUtf8*& shaderError) {
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError!=nullptr&&shaderError->GetStringLength()!=0) {
		ManagementLog::Log(shaderError->GetStringPointer());
		assert(false);
	}
}

void ManagementDXC::ReturnResult(const std::wstring& filePath, const wchar_t* profile, IDxcResult*& shaderResult, IDxcBlob*& shaderBlob, IDxcBlobEncoding*& shaderSource) {
	HRESULT hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したらログを出す
	ManagementLog::Log(ManagementLog::ConvertToUTF8(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
}

void ManagementDXC::GenerateRootSignature(ID3D12Device* device) {
	descriptionRootSignature_.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature_,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr)) {
		ManagementLog::Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}

	hr = device->CreateRootSignature(0,
		signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

void ManagementDXC::SettingInputLayout() {
	inputElementDescs_[0].SemanticName = "POSITION";
	inputElementDescs_[0].SemanticIndex = 0;
	inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
	inputLayoutDesc_.NumElements = _countof(inputElementDescs_);
}

void ManagementDXC::SettingBlendState() {
	blendDesc_.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
}

void ManagementDXC::SettingRasterizerState() {
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}

void ManagementDXC::CompilingShader() {
	vertexShaderBlob_ = CompileShader(L"Object3D.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = CompileShader(L"Object3D.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}

void ManagementDXC::GeneratePSO(ID3D12Device* device) {
	graphicspipelineStateDesc.pRootSignature = rootSignature_;//RootSignature
	graphicspipelineStateDesc.InputLayout = inputLayoutDesc_;//InputLayout
	graphicspipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(),
	vertexShaderBlob_->GetBufferSize() };//vertexShader
	graphicspipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(),
	pixelShaderBlob_->GetBufferSize() };//pixelShader
	graphicspipelineStateDesc.BlendState = blendDesc_;//BlendState
	graphicspipelineStateDesc.RasterizerState = rasterizerDesc;//RasterizerState
	//書き込むRTVの情報
	graphicspipelineStateDesc.NumRenderTargets = 1;
	graphicspipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ(形状)のタイプ。三角形
	graphicspipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面に色を打ち込むかの設定
	graphicspipelineStateDesc.SampleDesc.Count = 1;
	graphicspipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	HRESULT hr = device->CreateGraphicsPipelineState(&graphicspipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

void ManagementDXC::Release() {
	
	signatureBlob_->Release();
	if (errorBlob_) {
		errorBlob_->Release();
	}

	rootSignature_->Release();
	graphicsPipelineState->Release();
	pixelShaderBlob_->Release();
	vertexShaderBlob_->Release();
}