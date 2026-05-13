#include"ManagementViewPort.h"
#include<cassert>

void ManagementViewPort::Initialize(ID3D12Device* device, int kWindowWidth, int kWindowHeight) {
	GenerateVertexResource(device);
	GenerateVertexBufferView();
	Write2Resource();
	GenerateViewPort(kWindowWidth, kWindowHeight);
	CorrectionScissorRect(kWindowWidth, kWindowHeight);
}

void ManagementViewPort::GenerateVertexResource(ID3D12Device* device) {
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Vector4) * 3;//リソースのサイズ。今回はVector4を３頂点文
	//バッファの場合これらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));
}

void ManagementViewPort::GenerateVertexBufferView() {
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 3;
	//一個当たりの頂点サイズ
	vertexBufferView.StrideInBytes = sizeof(Vector4);
}

void ManagementViewPort::Write2Resource() {
	//書き込むためのアドレス取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0] = { -0.5f,-0.5f,0.0f,1.0f };
	//上
	vertexData[1] = { 0.0f,0.5f,0.0f,1.0f };
	//右下
	vertexData[2] = { 0.5f,-0.5f,0.0f,1.0f };

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
}