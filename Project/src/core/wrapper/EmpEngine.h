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
	static void GeneratePSO();
	static void SetBlendMode(BlendMode blendMode);
	static void SetRasterizer(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode);
	static void SetDepthStencil(bool depthEnable, D3D12_DEPTH_WRITE_MASK DepthWriteMask, D3D12_COMPARISON_FUNC comparisonFunc);
	static void SetPosition(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	static void SetTexCoord(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	static void SetNormal(const char* name, unsigned int index, DXGI_FORMAT format, UINT offset);
	static void SetVertexShader(const std::wstring& filePath);
	static void SetPixelShader(const std::wstring& filePath);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetWhite1x1();
	static void RebindRenderTarget();
};