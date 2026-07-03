#include"CameraController.h"
#include"intraction/Input/Input.h"
#include"DataModel/Matrix4x4.h"
#include"externals/imgui/imgui.h"


void DebugCamera::Initialize(Vector3 translate, Vector3 rotate) {
	translate_ = translate;
	rotate_ = rotate;

	mouseX = 0;
	mouseY = 0;
	preMouseX = 0;
	preMouseY = 0;
}

void DebugCamera::Update() {
#ifdef USE_IMGUI

	ImGui::Begin("Camera");
	ImGui::DragFloat3("cameraRotate", &rotate_.x, 0.01f, -1.0f, 10.0f, "%.3f", 0);
	ImGui::End();
#endif // USE_IMGUI

	RotateToMat();

	if (Input::GetInstance()->IsPressVk(MDK_W)) {
		translate_ += forward * kSpeed;
	}

	if (Input::GetInstance()->IsPressVk(MDK_A)) {
		translate_ -= right * kSpeed;
	}

	if (Input::GetInstance()->IsPressVk(MDK_S)) {
		translate_ -= forward * kSpeed;
	}

	if (Input::GetInstance()->IsPressVk(MDK_D)) {
		translate_ += right * kSpeed;
	}

	if (Input::GetInstance()->IsPressVk(MDK_E)) {
		translate_ += up * kSpeed;
	}
	if (Input::GetInstance()->IsPressVk(MDK_Q)) {
		translate_ -= up * kSpeed;
	}

	if (Input::GetInstance()->IsPressVk(MDK_R)) {
		translate_ = { 0.0f,0.0f,-10.0f };
		rotate_ = { 0.26f,0.0f,0.0f };
	}

}

void DebugCamera::RotateToMat() {
	Matrix4x4 rotateMatrix = Rotate(rotate_);

	right = {
		rotateMatrix.m[0][0],
		rotateMatrix.m[0][1],
		rotateMatrix.m[0][2],
	};

	up = {
		rotateMatrix.m[1][0],
		rotateMatrix.m[1][1],
		rotateMatrix.m[1][2],
	};

	forward = {
		rotateMatrix.m[2][0],
		rotateMatrix.m[2][1],
		rotateMatrix.m[2][2],
	};
}