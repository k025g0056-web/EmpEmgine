#pragma once
#include"Scene/Base/SceneBase.h"
#include<memory>
#include<d3d12.h>
#include"GameScene.h"
#include"appObj/Camera/Controller/CameraController.h"
#include"DataModel/TransForm3d.h"
#include"DataModel/Vector4.h"
#include"DataModel/ModelData.h"
#include"DataModel/SoundData.h"
#include"interaction/Draw/Model/Model.h"
#include<vector>
#include<array>

class GameManager :public Scene {

	struct OBJ {
		Transform3d transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		Model  model;
		ModelData modelData;
		Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
		Vector4 colorBuff = { 1.0f,1.0f,1.0f,1.0f };
	};

	struct SpritePar {
		D3D12_GPU_DESCRIPTOR_HANDLE textureHandle;
		Transform3d transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		Transform3d uvTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
		Vector4 colorBuff = { 1.0f,1.0f,1.0f,1.0f };
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

	// Sphere〜Multi_Materialの6種は、モデルデータを1回だけ読み込んでキャッシュしておく
	// (毎回ファイルを読み直すと重いので、複製元として使い回す)
	std::array<ModelData, 6> modelDataCache_{};
	std::array<bool, 6> modelDataLoaded_{};

	struct ModelPath {
		const char* directory;
		const char* filename;
	};
	// Sphere, Stanford_Bunny, Plane, UtahTeapot, Multi_Mesh, Multi_Material の順
	const ModelPath modelPaths_[6] = {
		{ "resources/3dObject/sphere",        "sphere.obj" },
		{ "resources/3dObject/bunny",         "bunny.obj" },
		{ "resources/3dObject/plane",         "plane.obj" },
		{ "resources/3dObject/teapot",        "teapot.obj" },
		{ "resources/3dObject/multiMesh",     "multiMesh.obj" },
		{ "resources/3dObject/multiMaterial", "multiMaterial.obj" },
	};

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

	// 選択中のモデル種別に応じて、新規OBJ(またはSprite)を1体追加する
	void CreateSelectedModel();

	// Sphere〜Multi_Materialの.objを(初回だけ)読み込んでキャッシュを返す
	const ModelData& GetOrLoadModelData(Models modelType);

	// OBJ1体分のトランスフォームをImGuiで編集するUI(共通化)
	// 戻り値: 削除ボタンが押されたらtrue
	bool EditObjGui(const char* label, int index, OBJ& obj);

	// Sprite1体分のトランスフォーム・UVトランスフォームを編集するUI
	bool EditSpriteGui(int index, SpritePar& sprite);

	// vector<OBJ>を一覧表示・編集するヘルパー
	void DrawObjListGui(const char* categoryLabel, std::vector<OBJ>& objList);

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