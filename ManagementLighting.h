#pragma once
#include"DirectionalLight.h"
#include<d3d12.h>

class ManagementLighting {
	ID3D12Resource* directionalLightResource_=nullptr;
	DirectionalLight* directionalLightData=nullptr;

public:
	void Initialize(ID3D12Device* device);
	void DrawCall(ID3D12GraphicsCommandList* commandList);
	void GUI();
	void Release();
};