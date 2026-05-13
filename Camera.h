#pragma once
#include"TransForm3d.h"
#include"MatrixFunction.h"
#include"Matrix4x4.h"

class Camera {
	Transform3d cameraTransform;
	Matrix4x4 worldViewProjection;
	Matrix4x4 transformationMatrixData;
public:
	void Initialize();
	void Update(Transform3d transform, int kWindowWidth, int kWindowHeight);
	Matrix4x4 GetTransformationMatrixData() { return transformationMatrixData; }
};