#include"Camera3d.h"

void Camera3d::Initialize() {
	cameraTransform ={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
}

void Camera3d::Update(int kWindowWidth, int kWindowHeight) {
	Matrix4x4 cameraMatrix = Affine(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFov(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
	ViewProjection = Multiply(viewMatrix, projectionMatrix);

	Matrix4x4 viewSprite = MakeIdentity4x4();
	Matrix4x4 projecSprite = MakeOrthographic(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 100.0f);
	ViewProjectionSprite = Multiply(viewSprite, projecSprite);

}

Matrix4x4 Camera3d::GetWvp(const Transform3d& transform) const {
	Matrix4x4 worldMatrix = Affine(transform);
	return Multiply(worldMatrix, ViewProjection);
}

Matrix4x4 Camera3d::GetWvpSprite(const Transform3d& transform) const {
	Matrix4x4 worldMatrix = Affine(transform);
	return Multiply(worldMatrix, ViewProjectionSprite);
}