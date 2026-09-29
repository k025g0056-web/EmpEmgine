#pragma once
#include"Interaction/Draw/Shape/Shape.h"
#include"appObj/Camera/Camera3d/Camera3d.h"

//こいつは課題の残り香をぷんぷんさせるクラスなのら
class DoubleTriangle :public Shape {
	bool enableLight = false;
public:
	void Initialize(ID3D12Device* device);

	void Draw(ID3D12GraphicsCommandList* commandList, const Camera3d& camera, const Transform3d& transform,
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);

	bool GetEnableLight() { return enableLight; }
	void SetEnableLight(bool enable) { enableLight = enable; }
};