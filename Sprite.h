#pragma once
#include"Shape.h"
#include"Vector2.h"
#include"Camera3d.h"
#include"Matrix4x4.h"

class Sprite :public Shape{
	Vector2 vec_[4]{};


public:
	void Initialize(ID3D12Device* device);
	void DrawSprite(const Transform3d& transform,
		const Vector2& v0, const Vector2& v1, const Vector2& v2, const Vector2& v3, ID3D12GraphicsCommandList* commandList,
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Vector4& color, const Camera3d& camera,const Transform3d& uvTransform);
};