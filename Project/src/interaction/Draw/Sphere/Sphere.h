#pragma once
#include"Interaction/Draw/Shape/Shape.h"
#include"appObj/Camera/Camera3d/Camera3d.h"

class Sphere :public Shape{
	const int kSubdivision = 16;
public:
	void Initialize(ID3D12Device* device);
	void Draw(ID3D12GraphicsCommandList* commandList, const Camera3d& camera,
		const Transform3d& transform, const Vector4& color,D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);
};