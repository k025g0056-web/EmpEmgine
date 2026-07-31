#pragma once
#include"Scene/Base/SceneBase.h"
#include<memory>//どうせ後々使うので入れておく
#include<d3d12.h>
#include"GameScene.h"
#include"appObj/Camera/Controller/CameraController.h"
#include"DataModel/TransForm3d.h"
#include"DataModel/Vector4.h"
#include"DataModel/ModelData.h"
#include"DataModel/SoundData.h"
#include"interaction/Draw/Model/Model.h"
#include<vector>

class GameManager :public Scene {

	struct OBJ{
		Transform3d transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		Model  model;
		ModelData modelData;
		Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
		Vector4 colorBuff = {1.0f,1.0f,1.0f,1.0f};
	};

	struct SpritePar{
		D3D12_GPU_DESCRIPTOR_HANDLE textureHandle;
		Transform3d transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		Transform3d uvTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		Vector4 color= { 1.0f,1.0f,1.0f,1.0f };
		Vector4 colorBuff = {1.0f,1.0f,1.0f,1.0f};
		Vector2 point0 = { 0.0f,360.0f };
		Vector2 point1 = { 0.0f,0.0f };
		Vector2 point2 = { 640.0f,360.0f };
		Vector2 point3 = { 640.0f,0.0f };
	};

	//課題用の変数
	//----------------------------------------------------------------------------//
	D3D12_GPU_DESCRIPTOR_HANDLE uvChecker_{};
	D3D12_GPU_DESCRIPTOR_HANDLE monsterBall_{};
	D3D12_GPU_DESCRIPTOR_HANDLE checkerBoard_{};

	std::vector<OBJ> suzanne_{};
	std::vector<OBJ> sphere_{};
	std::vector<OBJ> stanfordBunny_{};
	std::vector<OBJ> plane_{};
	std::vector<OBJ> utahTeaPot_{};
	std::vector<OBJ> multiMesh_{};
	std::vector<OBJ> multiMaterial_{};
	std::vector<SpritePar> sprite_{};

	const char* modelTable[7] = {
	"Sprite",
	"Sphere",
	"Stanford_Bunny",
	"Plane",
	"UtahTeapot",
	"Multi_Mesh",
	"Multi_Material"
	};

	enum Models {
		Sprite,
		Sphere,
		Stanford_Bunny,
		Plane,
		UtahTeapot,
		Multi_Mesh,
		Multi_Material
	};

	Models models = Sprite;

	int modelIndex_ = 0;

	//---------------------------------------------------------------------------//

	void DrawHomeWork();
	void UpdateHomeWork();
	void GuiHomeWork();
public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

	GameManager();
	~GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
private:
	DebugCamera debugCamera_;
	void GUI();
};
