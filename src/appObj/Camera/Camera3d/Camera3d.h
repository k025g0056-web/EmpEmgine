#pragma once
#include"DataModel/TransForm3d.h"
#include"DataModel/Matrix4x4.h"

class Camera3d {
	Transform3d cameraTransform;
	Matrix4x4 ViewProjection;
	Matrix4x4 ViewProjectionSprite;
public:
	void Initialize();
	void Update(int kWindowWidth, int kWindowHeight);
	
	Matrix4x4 GetWvp(const Transform3d& transform) const ;
	Matrix4x4 GetWvpSprite(const Transform3d& transform) const;
	Vector3& GetCameraPosition() { return cameraTransform.translate; }
	Vector3& GetCameraRotate() { return cameraTransform.rotate; }
	void SetCameraPosition(Vector3 position) { cameraTransform.translate = position; }
	void SetCameraRotate(Vector3 rotate) { cameraTransform.rotate = rotate; }
};