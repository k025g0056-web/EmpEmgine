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

	// ↓ 追加：当たり判定用の球（往復移動するだけの簡単なもの）
	Transform3d ballTransform_{ {0.1f,0.1f,0.1f}, {0.0f,0.0f,0.0f}, {-4.0f,0.0f,0.0f} };
	Vector3     ballVelocity_{ 2.0f, 0.0f, 0.0f };
	float       ballRadius_ = 1.0f;
	Vector3 ballStartPos_{ -6.0f,0.0f,0.0f };
	float weight = 100.0f; // kg見立て。潰れの最大量とふくらみ量を決める
	enum class BallAxis
	{
		X,
		Y,
		Z
	};

	BallAxis ballAxis_ = BallAxis::X;
	bool isBallMove_ = false;
	bool hitOnce_ = false;
	float hitDistance_ = 2.0f;
	//---------------------------------------------------------------------------//

	void DrawHomeWork();
	void UpdateHomeWork();
	void GuiHomeWork();
	void UpdateBall(float dt);      // ← 追加：球を往復させるだけの簡単な移動
	void DrawBall();                // ← 追加：球を描画
	bool CheckHitModel() const;     // ← 追加：ここに実際の当たり判定を書く
	void TryCompressOnHit();        // ← 追加：当たったら「たまに」潰す抽選＆発動
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