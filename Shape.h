#pragma once
#include <d3d12.h>
#include"Vector4.h"
#include"Matrix4x4.h"
#include"VertexData.h"
#include"TransForm3d.h"
#include"Material.h"
#include"TransformationMatrix.h"

//図形のクラス。いい感じに作りたい
class Shape {
protected:
	ID3D12Resource* vertexResource = nullptr;
	ID3D12Resource* materialResource = nullptr;
	Material* materialData = nullptr;
	ID3D12Resource* wvpResource = nullptr;
	TransformationMatrix* wvpData = nullptr;
	Transform3d transform3d_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	bool isVertexDirty_ = true;
	bool isTransformDirty_ = true;

	void GenerateMaterial(ID3D12Device* device,const Vector4& color,bool enableLighting);
	void GenerateWvpResource(ID3D12Device* device);
	void DrawCall(ID3D12GraphicsCommandList* commandList,
		D3D12_VERTEX_BUFFER_VIEW vetexBufferView, 
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, int vertex);

	void SetColor(const Vector4& color);
	void ChangeTransform(const Transform3d& transform);
public:
	void Initialize(ID3D12Device* device,bool enableLighting);
	void Release();
};