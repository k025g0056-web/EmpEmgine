#pragma once
#define WIN32_LEAN_AND_MEAN
#include"EmpSysetms.h"
#include"Vector3.h"
#include"Vector4.h"
#include"SceneManager.h"

extern EmpSystems empSystems; // グローバル変数の宣言なの！

extern SceneManager sceneManager_;

class EmpEngine {
	
public:
	static void Initialize(int kWindowWidth, int kWindowHeight, SceneName scene);
	static int ProcessMessage();
	static void SetWindowSize(unsigned int index, int windowWidth, int windowHeight);
	static void Finalize();
	static void Begin();
	static void DrawTriangle(Vector3 v0,Vector3 v1,Vector3 v2,Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);
	static void DrawTriangle(Transform3d transform, Vector3 v0, Vector3 v1, Vector3 v2, Vector4 color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);
	static void DrawSprite(const Transform3d& transform, const Vector2& v0,
		const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector4& color, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);
	static void DrawSphere(const Transform3d& transform, const Vector4& color
		, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);
	static void End();
	static bool EndManagement();
	static void Process();
	static void LightGUI();
	static D3D12_GPU_DESCRIPTOR_HANDLE LoadTexture(const std::string& str);

};