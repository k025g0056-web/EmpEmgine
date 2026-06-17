#pragma once
#include "Shape.h"
#include"ModelData.h"
#include"TransForm3d.h"
#include"Camera3d.h"

class Model :public Shape{
	ModelData modelData_;
public:
	void Initialize(ID3D12Device* device,const ModelData& modelData);
	void DrawModel(const Transform3d& transform, ID3D12GraphicsCommandList* commandList,
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Vector4& color, const Camera3d& camera);
};

