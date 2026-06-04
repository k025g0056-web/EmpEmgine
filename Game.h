#pragma once
#include"SceneBase.h"
#include<memory>//どうせ後々使うので入れておく
#include<d3d12.h>
#include"TransForm3d.h"
#include"Vector4.h"
#include"TransForm3d.h"
#include"Vector4.h"

//クラスで関数を作ってから入れるのだ
class GameManager :public Scene {
	//課題用の変数
	//----------------------------------------------------------------------------//
	D3D12_GPU_DESCRIPTOR_HANDLE uvChecker{};
	D3D12_GPU_DESCRIPTOR_HANDLE monsterBall{};
	D3D12_GPU_DESCRIPTOR_HANDLE sphereHandle{};
	bool useMonsterBall = true;
	Transform3d transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform3d transformSpr{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Vector4 SprColor{ 1.0f,1.0f,1.0f,1.0f };
	//---------------------------------------------------------------------------//
	void DrawHomeWork();
public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

	GameManager();
	~GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
};
