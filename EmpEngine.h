#pragma once
#define WIN32_LEAN_AND_MEAN
#include"EmpSysetms.h"
#include"Vector3.h"
#include"Vector4.h"

extern EmpSystems empSystems; // グローバル変数の宣言なの！


class EmpEngine {
public:
	static void Initialize(int kWindowWidth, int kWindowHeight);
	static int ProcessMessage();
	static void SetWindowSize(unsigned int index, int windowWidth, int windowHeight);
	static void Finalize();
	static void Begin();
	static void DrawTriangleViewPort();
	static void DrawTriangle(Vector3 v0,Vector3 v1,Vector3 v2,Vector4 color);
	static void DrawTriangle(Transform3d transform, Vector3 v0, Vector3 v1, Vector3 v2, Vector4 color);
	static void End();
	static void Update();

};