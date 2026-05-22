#pragma once
#define WIN32_LEAN_AND_MEAN
#include"EmpSysetms.h"

extern EmpSystems empSystems; // グローバル変数の宣言なの！


class EmpEngine {
public:
	static void Initialize(int kWindowWidth, int kWindowHeight);
	static int ProcessMessage();
	static void SetWindowSize(unsigned int index, int windowWidth, int windowHeight);
	static void Finalize();
	static void Begin();
	static void DrawTriangle();
	static void End();
	static void Update();

};