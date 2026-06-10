#include"Shape.h"
#include"DX12Mechanics.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

void Shape::Initialize(ID3D12Device* device,bool enableLighting) {
	GenerateWvpResource(device);
	GenerateMaterial(device, Vector4(1.0f, 1.0f, 1.0f, 1.0f),enableLighting);
}

void Shape::GenerateMaterial(ID3D12Device* device,const Vector4& color,bool enableLighting) {
	//マテリアル用のリソースを作る
	materialResource = DX12Mechanics::CreateBufferResource(device, sizeof(Material), 256);

	//書き込む溜めのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	materialData->color = color;

	materialData->enableLighting = enableLighting;

	materialData->uvTransform = MakeIdentity4x4();

}

void Shape::GenerateWvpResource(ID3D12Device * device) {
	wvpResource = DX12Mechanics::CreateBufferResource(device, sizeof(TransformationMatrix), 256);
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	//単位行列を書き込む
	wvpData->WVP = MakeIdentity4x4();
	wvpData->world = MakeIdentity4x4();

}

void Shape::DrawCallVertex(ID3D12GraphicsCommandList* commandList,
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView, D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,int vertex) {
	SetUpDrawCall(commandList, vertexBufferView, textureSrvHandleGPU);
	//描画！（DrawCall/ドローコール）。3頂点で１つのインスタンス。インスタンスについては今後
	commandList->DrawInstanced(vertex, 1, 0, 0);
}

void Shape::DrawCallIndex(ID3D12GraphicsCommandList* commandList,
	D3D12_INDEX_BUFFER_VIEW indexBufferView, D3D12_VERTEX_BUFFER_VIEW vertexBufferView,
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, int index) {
	commandList->IASetIndexBuffer(&indexBufferView);//IBVを設定
	SetUpDrawCall(commandList, vertexBufferView, textureSrvHandleGPU);
	commandList->DrawIndexedInstanced(index, 1, 0, 0,0);
}

void Shape::SetColor(const Vector4& color) {
	materialData->color = color;
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

void Shape::SetUpDrawCall(ID3D12GraphicsCommandList* commandList,
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView, D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU) {
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じ物を設定すると考えて置けば良い
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
}