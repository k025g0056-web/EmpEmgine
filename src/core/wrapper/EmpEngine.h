#pragma once
#define WIN32_LEAN_AND_MEAN
#include"core/internal/EmpSysetms.h"
#include"DataModel/Vector3.h"
#include"DataModel/Vector4.h"
#include"Scene/Manager/SceneManager.h"
#include"DataModel/ModelData.h"

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
		const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector4& color,
		D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle, const Transform3d& uvTransform);
	static void DrawSphere(const Transform3d& transform, const Vector4& color
		, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle);
	static void DrawTriangleColor(const Vector3& v0, const Vector3& v1,
		const Vector3& v2, const Vector4 color);
	static void DrawTriangleColor(const Transform3d& transform, const Vector3& v0, const Vector3& v1,
		const Vector3& v2, const Vector4 color);
	static void End();
	static bool EndManagement();
	static void Process();
	static void LightGUI();
	static D3D12_GPU_DESCRIPTOR_HANDLE LoadTexture(const std::string& str);
	static void SetWindowColor(Vector4 color);
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	static void DrawPreModel(const Transform3d& transform, D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle, const ModelData& modelData_);
	static SoundData SoundLoadWave(const char* filename);
	static void PlayAudio(const SoundData& soundData);
	static void UnLoadAudio(SoundData* soundData);
	static void DrawLight();
	// EmpEngine 経由で呼べるようにするなの
	static ID3D12Device* GetDevice();
	static void DrawCompressModel(
		const Transform3d& transform,
		D3D12_GPU_DESCRIPTOR_HANDLE GraphHandle,
		Model& model);
};