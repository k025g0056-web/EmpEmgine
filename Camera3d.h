#pragma once
#include"TransForm3d.h"
#include"Matrix4x4.h"

class Camera3d {
	Transform3d cameraTransform;
	Matrix4x4 ViewProjection;
public:
	void Initialize();
	void Update(int kWindowWidth, int kWindowHeight);
	
	Matrix4x4 GetWvp(const Transform3d& transform) const ;
};