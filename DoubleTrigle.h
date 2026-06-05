#pragma once
#include"TransForm3d.h"
#include<d3d12.h>

class DoubleTrigle{
	Transform3d trian1_;
	Vector3 verticeTri_[3];
	Transform3d trian2_;
	D3D12_GPU_DESCRIPTOR_HANDLE uvChecker_;
	D3D12_GPU_DESCRIPTOR_HANDLE monsterBall_;
	D3D12_GPU_DESCRIPTOR_HANDLE oirano_;
	D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle1_;
	D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle2_;
public:
	void Initialize();

	void GUI();

	void Draw();
};