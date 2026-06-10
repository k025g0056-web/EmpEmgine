#pragma once
#include <d3d12.h>
#include"Vector4.h"
#include"Matrix4x4.h"
#include"VertexData.h"
#include"TransForm3d.h"
#include"Material.h"
#include"TransformationMatrix.h"
#include"Matrix3x3.h"

//図形のクラス。いい感じに作りたい
class Shape {
private:
	void SetUpDrawCall(ID3D12GraphicsCommandList* commandList,
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView, D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);
protected:
	ID3D12Resource* materialResource = nullptr;
	Material* materialData = nullptr;

	ID3D12Resource* wvpResource = nullptr;
	TransformationMatrix* wvpData = nullptr;
	
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ID3D12Resource* vertexResource = nullptr;

	ID3D12Resource* indexResource_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	Transform3d transform3d_;

	bool isVertexDirty_ = true;
	bool isTransformDirty_ = true;

	void GenerateMaterial(ID3D12Device* device,const Vector4& color,bool enableLighting);
	void GenerateWvpResource(ID3D12Device* device);
	void DrawCallVertex(ID3D12GraphicsCommandList* commandList,
		D3D12_VERTEX_BUFFER_VIEW vetexBufferView, 
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, int vertex);
	void DrawCallIndex(ID3D12GraphicsCommandList* commandList,
		D3D12_INDEX_BUFFER_VIEW indexBufferView, D3D12_VERTEX_BUFFER_VIEW vertexBufferView,
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, int index);

	void SetColor(const Vector4& color);
	void ChangeTransform(const Transform3d& transform);
	void SetUvTransForm(const Transform3d& uvTransform) { 
		Matrix4x4 uvTransformMatrix = Affine3D22D(uvTransform);
		materialData->uvTransform = uvTransformMatrix; }
public:
	void Initialize(ID3D12Device* device,bool enableLighting);
	void Release();
};