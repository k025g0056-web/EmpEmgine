#include"ManagementDXC.h"
#pragma comment(lib,"dxcompiler.lib")
#include<cassert>
#include"Log/ManagementLog.h"
#include<format>

void ManagementDXC::Initialize(ID3D12Device* device) {
	device_ = device;

	GenerateInstance();
	SettingHandler();
	GenerateRootSignature();
	SetPosition("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);
	SetTexcoord("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);
	SetNormal("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);
	SetBlendMode(kBlendModeNone);
	SetRasterizer(D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_SOLID);
	SetDepthStencil(true, D3D12_DEPTH_WRITE_MASK_ALL, D3D12_COMPARISON_FUNC_LESS_EQUAL);
	SetVertexShader(L"Object3D.VS.hlsl");
	SetPixelShader(L"Object3D.PS.hlsl");
	GeneratePSO();
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

	std::wstring file = L"resources/shader/" + filePath;

	//1.hlslファイルを読む
	LoadingFile(file, profile,shaderSource,shaderSourceBuffer);
	//2.Compileする
	Compiling(file, profile,shaderSourceBuffer,shaderResult);
	//3.警告・エラーが出てないか確認する
	CheckingError(shaderResult,shaderError);
	//4.Compile結果を受け取って返す
	ReturnResult(file,profile,shaderResult,shaderBlob,shaderSource);
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
		includeHandler_.Get(),//インクルードが含まれた諸々
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

void ManagementDXC::GenerateRootSignature() {

	//この関数で使う変数の初期化
	//----------------------------------------------------------//
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	ID3DBlob* signatureBlob_ = nullptr;
	ID3DBlob* errorBlob_ = nullptr;
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//------------------------------------------------------------//

	//ディスクリプタレンジの設定
	//-------------------------------------------------------------//
	descriptorRange[0].BaseShaderRegister = 0;//0から始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算
	//-------------------------------------------------------------//

	//ルートシグネチャのフラグ
	descriptionRootSignature_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//ピクセルシェーダーの設定
	//---------------------------------------------------------------------------//
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド
	//---------------------------------------------------------------------------//

	//バーテックスシェーダーの設定
	//-------------------------------------------------------------------------//
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド
	//-------------------------------------------------------------------------//

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Descriptor tableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderを使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;//tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//tableで利用する数

	//ライティングの設定
	//------------------------------------------------------------------------//
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//Pixelshader
	rootParameters[3].Descriptor.ShaderRegister = 1;//レジスタ番号１を使う
	//------------------------------------------------------------------------//

	descriptionRootSignature_.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootSignature_.NumParameters = _countof(rootParameters);//配列の長さ

	//サンプラーの設定
	//------------------------------------------//
	staticSamplers[0].Filter = D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature_.pStaticSamplers = staticSamplers;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);
	//------------------------------------------//

	//ここでミス検知
	//-----------------------------------------------------------------------//
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature_,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr)) {
		ManagementLog::Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}

	hr = device_->CreateRootSignature(0,
		signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
	//-----------------------------------------------------------------------//


	//ここでしか使わないメモリの解放
	//-----------------------------------------//
	signatureBlob_->Release();
	if (errorBlob_) {
		errorBlob_->Release();
	}
	//-----------------------------------------//

}

void ManagementDXC::GeneratePSO() {

	if (!isDirty_) {
		return;
	}

	//関数で使う変数の初期化
	//--------------------------------------------------//
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	//---------------------------------------------------//

	//インプットレイヤーの設定
	//====================================================================================//
	
	inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
	inputLayoutDesc_.NumElements = _countof(inputElementDescs_);

	//================================================================================//

	//Depth Stencilの設定
	//-------------------------------------------------------------------//
	
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc_;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleDesc.Quality = 0;
	//-------------------------------------------------------------------//

	//シェーダーのコンパイル
	//---------------------------------------------------------//
	//----------------------------------------------------------//

	graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();//RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;//InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(),
	vertexShaderBlob_->GetBufferSize() };//vertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(),
	pixelShaderBlob_->GetBufferSize() };//pixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc_;//BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;//RasterizerState
	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面に色を打ち込むかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	HRESULT hr = device_->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));

	isDirty_ = false;
}

void ManagementDXC::Release() {
	rootSignature_.Reset();
	graphicsPipelineState_.Reset();
}

void ManagementDXC::SetBlendMode(BlendMode blendMode) {
	//ブレンドの状態の設定
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc_.RenderTarget[0].BlendEnable = TRUE;
	switch (blendMode) {
	case kBlendModeNone:
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		break;
	case kBlendModeNormal:
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		break;
	case kBlendModeAdd:
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

		break;
	case kBlendModeSubtract:
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

		break;
	case kBlendModeMultiply:
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;

		break;
	case kBlendModeScreen:
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

		break;
	case kCountOfBlendMode:
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		break;
	default:
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		break;
	}

	blendDesc_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc_.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	isDirty_ = true;
}

void ManagementDXC::SetRasterizer(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode) {
	//リスタライザーの設定
	//----------------------------------------------------//
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = cullMode;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = fillMode;
	//-----------------------------------------------------//

	isDirty_ = true;
}

void ManagementDXC::SetDepthStencil(bool depthEnable, D3D12_DEPTH_WRITE_MASK DepthWriteMask, D3D12_COMPARISON_FUNC comparisonFunc) {
	//Depthの機能を有効化する
	depthStencilDesc_.DepthEnable = depthEnable;

	//書き込みします
	depthStencilDesc_.DepthWriteMask = DepthWriteMask;

	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc_.DepthFunc = comparisonFunc;

	isDirty_ = true;

}

void ManagementDXC::SetPosition(const char* name,unsigned int index, DXGI_FORMAT format,UINT offset) {
	//ポジションの設定
	//-----------------------------------------------------------//
	inputElementDescs_[0].SemanticName = name;
	inputElementDescs_[0].SemanticIndex = index;
	inputElementDescs_[0].Format = format;
	inputElementDescs_[0].AlignedByteOffset = offset;
	//--------------------------------------------------------------//

	isDirty_ = true;
}

void ManagementDXC::SetTexcoord(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset) {
	//テックスコードの設定
	//-----------------------------------------------------------//
	inputElementDescs_[1].SemanticName = name;
	inputElementDescs_[1].SemanticIndex = index;
	inputElementDescs_[1].Format = format;
	inputElementDescs_[1].AlignedByteOffset = offset;
	//-------------------------------------------------------------//

	isDirty_ = true;
}

void ManagementDXC::SetNormal(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset) {
	//法線の設定
	//-----------------------------------------------------------//
	inputElementDescs_[2].SemanticName = name;
	inputElementDescs_[2].SemanticIndex = index;
	inputElementDescs_[2].Format = format;
	inputElementDescs_[2].AlignedByteOffset = offset;
	//-----------------------------------------------------------//

	isDirty_ = true;
}

void ManagementDXC::SetVertexShader(const std::wstring& filePath) {
	vertexShaderBlob_= CompileShader(filePath, L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	isDirty_ = true;
}

void ManagementDXC::SetPixelShader(const std::wstring& filePath) {
	pixelShaderBlob_ = CompileShader(filePath, L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
	isDirty_ = true;
}