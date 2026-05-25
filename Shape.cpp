#include"Shape.h"
#include"DX12Mechanics.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

void Shape::Initialize(ID3D12Device* device) {
	GenerateWvpResource(device);
	GenerateMaterial(device, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Shape::GenerateMaterial(ID3D12Device* device,const Vector4& color) {
	//マテリアル用のリソースを作る
	materialResource = DX12Mechanics::CreateBufferResource(device, sizeof(Vector4), 256);

	//書き込む溜めのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//今回は赤を書き込んでみる
	*materialData = color;
}

void Shape::GenerateWvpResource(ID3D12Device * device) {
	wvpResource = DX12Mechanics::CreateBufferResource(device, sizeof(Matrix4x4), 256);
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	//単位行列を書き込む
	*wvpData = MakeIdentity4x4();
}

void Shape::DrawCall(ID3D12GraphicsCommandList* commandList,
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView, D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,int vertex) {
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じ物を設定すると考えて置けば良い
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
	//描画！（DrawCall/ドローコール）。3頂点で１つのインスタンス。インスタンスについては今後
	commandList->DrawInstanced(vertex, 1, 0, 0);
}

void Shape::SetColor(const Vector4& color) {
	*materialData = color;
}

void Shape::Release() {
	vertexResource->Release();
	materialResource->Release();
	wvpResource->Release();
}

void Shape::ChangeTransform(const Transform3d& transform) {
	if (transform3d_ == transform) {
		isTransformDirty_ = false;
	}
	else {
		isTransformDirty_ = true;
		transform3d_ = transform;
	}
}