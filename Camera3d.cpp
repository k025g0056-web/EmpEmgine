#include"Camera3d.h"

void Camera3d::Initialize() {
	cameraTransform ={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	transformationMatrixData = {};
}

void Camera3d::Update(Transform3d transform, int kWindowWidth, int kWindowHeight) {
	Matrix4x4 worldMatrix = Affine(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = Affine(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFov(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
	worldViewProjection = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData = worldViewProjection;
}