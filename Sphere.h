#pragma once
#include"Shape.h"
#include"Camera3d.h"

class Sphere :public Shape{
	const int kSubdivision = 16;
public:
	void Initialize(ID3D12Device* device);
	void DrawSphere(const Transform3d& transform, const Vector4& color, ID3D12GraphicsCommandList* commandList,
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Camera3d& camera);
};