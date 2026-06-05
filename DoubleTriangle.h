#pragma once
#include"Shape.h"
#include"Camera3d.h"

//こいつは課題の残り香をぷんぷんさせるクラスなのら
class DoubleTriangle :public Shape {
	bool enableLight = false;
public:
	void Initialize(ID3D12Device* device);

	void DrawDoubleTriangle(const Transform3d& transform, ID3D12GraphicsCommandList* commandList,
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU, const Camera3d& camera);

	bool GetEnableLight() { return enableLight; }
	void SetEnableLight(bool enable) { enableLight = enable; }
};