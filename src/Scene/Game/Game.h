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
#include"intraction/Draw/Model/Model.h"

class GameManager :public Scene {
	//課題用の変数
	//----------------------------------------------------------------------------//
	D3D12_GPU_DESCRIPTOR_HANDLE uvChecker{};
	D3D12_GPU_DESCRIPTOR_HANDLE monsterBall{};
	D3D12_GPU_DESCRIPTOR_HANDLE sphereHandle{};
	ModelData modelData_;
	bool useMonsterBall = true;
	Transform3d transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform3d transformSpr{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform3d uvTransformSpr{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Vector4 SprColor{ 1.0f,1.0f,1.0f,1.0f };
	Vector4 colorBuff = {};
	SoundData Alarm01_;

	Model  model_;
	bool   compressStarted_ = false;
	// deltaTime_ を追加なの
	float deltaTime_ = 0.0f;
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
