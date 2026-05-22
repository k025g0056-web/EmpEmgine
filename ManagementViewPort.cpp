#include"ManagementViewPort.h"
#include<cassert>

void ManagementViewPort::Initialize(ID3D12Device* device, int kWindowWidth, int kWindowHeight) {
	vertexResource = DX12Mechanics::CreateBufferResource(device, sizeof(VertexData) * 6, WhichResource::Vertex);
	GenerateVertexBufferView();
	Write2Resource();
	GenerateViewPort(kWindowWidth, kWindowHeight);
	CorrectionScissorRect(kWindowWidth, kWindowHeight);
	GenerateMaterial(device);
	GenerateWvpResource(device);
	depthStencilResource_=CreateDepthStencilTextureResource(device, kWindowWidth, kWindowHeight);
}

void ManagementViewPort::GenerateVertexBufferView() {
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	//一個当たりの頂点サイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

void ManagementViewPort::Write2Resource() {
	//書き込むためのアドレス取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//左下
	vertexData_[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData_[0].texcoord = { 0.0f,1.0f };
	//上
	vertexData_[1].position = { 0.0f,0.5f,0.0f,1.0f };
	vertexData_[1].texcoord = { 0.5f,0.0f };
	//右下
	vertexData_[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData_[2].texcoord = { 1.0f,1.0f };

	//左下
	vertexData_[3].position = { -0.5f,-0.5f,0.5f,1.0f };
	vertexData_[3].texcoord = { 0.0f,1.0f };
	//上
	vertexData_[4].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData_[4].texcoord = { 0.5f,0.0f };
	//右下
	vertexData_[5].position = { 0.5f,-0.5f,-0.5f,1.0f };
	vertexData_[5].texcoord = { 1.0f,1.0f };
}

void ManagementViewPort::GenerateViewPort(int kWindowWidth, int kWindowHeight) {
	viewport.Width = static_cast<float>(kWindowWidth);
	viewport.Height = static_cast<float>(kWindowHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void ManagementViewPort::CorrectionScissorRect(int kWindowWidth, int kWindowHeight) {
	//基本的にビューポートと同じ形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = kWindowWidth;
	scissorRect.top = 0;
	scissorRect.bottom = kWindowHeight;
}

void ManagementViewPort::Release() {
	vertexResource->Release();
	materialResource->Release();
	wvpResource->Release();
}

void ManagementViewPort::GenerateMaterial(ID3D12Device* device) {
	//マテリアル用のリソースを作る
	materialResource = DX12Mechanics::CreateBufferResource(device, sizeof(Vector4),WhichResource::Constant);

	//書き込む溜めのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//今回は赤を書き込んでみる
	*materialData = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

void ManagementViewPort::GenerateWvpResource(ID3D12Device* device) {
	wvpResource = DX12Mechanics::CreateBufferResource(device, sizeof(Matrix4x4),WhichResource::Constant);
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	//単位行列を書き込む
	*wvpData = MakeIdentity4x4();
}

void ManagementViewPort::Update(Camera3d camera) {
	transform.rotate.y += 0.03f;
	*wvpData = camera.GetTransformationMatrixData();
}

ID3D12Resource* ManagementViewPort::CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height) {
	//生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;//Textureの幅
	resourceDesc.Height = height;//Textureの高さ
	resourceDesc.MipLevels = 1;//mipmapの数
	resourceDesc.DepthOrArraySize = 1;//奥行きor配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;//サンプリングカウント。1固定。
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//DepthStencilとして扱う通知

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;//vRAM上に作る

	//深度のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//フォーマット。Resourceと合わせる。

	//Resourceの設定
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//Heapの特殊な設定。特になし
		&resourceDesc,//Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値を書き込む状態にしておく
		&depthClearValue,//Clear最適値
		IID_PPV_ARGS(&resource));//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));
	return resource;
}