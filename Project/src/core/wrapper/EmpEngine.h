#pragma once
#define WIN32_LEAN_AND_MEAN
#include"core/internal/EmpSystems.h"
#include"DataModel/Vector3.h"
#include"DataModel/Vector4.h"
#include"Scene/Manager/SceneManager.h"
#include"DataModel/ModelData.h"

class EmpEngine {
	
public:
	static void Initialize(int kWindowWidth, int kWindowHeight, SceneName scene);
	static int ProcessMessage();
	static void SetWindowSize(unsigned int index, int windowWidth, int windowHeight);
	static void Finalize();
	static void Begin();

	
	static void End();
	static bool EndManagement();
	static void Process();
	static void LightGUI();
	static void PostDraw();
	
	static LoaderManager& Resource();
	static DrawManager& Draw();
	static void SetWindowColor(Vector4 color);
	static void DrawLight();
	// EmpEngine 経由で呼べるようにするなの
	static ID3D12Device* GetDevice();
	static D3D12_GPU_DESCRIPTOR_HANDLE GetWhite1x1();
	static void RebindRenderTarget();
	static float DeltaTime();
	static std::mt19937& GetRandom();
};