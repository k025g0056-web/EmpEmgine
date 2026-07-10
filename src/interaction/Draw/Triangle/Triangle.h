#pragma once
#include"Interaction/Draw/Shape/Shape.h"
#include"DataModel/Vector3.h"
#include"DataModel/TransForm3d.h"
#include"appObj/Camera/Camera3d/Camera3d.h"

class Triangle :public Shape {
	Vector3 v0_;
	Vector3 v1_;
	Vector3 v2_;

	void ChangeVertex(const Vector3& v0, const Vector3& v1, const Vector3& v2);

public:
	void Initialize(ID3D12Device* device);

	void DrawTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, ID3D12GraphicsCommandList* commandList,
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU,const Vector4& color);

	void DrawTriangle(const Transform3d& transform,const Vector3& v0, const Vector3& v1, const Vector3& v2, ID3D12GraphicsCommandList* commandList,
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Vector4& color,const Camera3d& camera);
};