#pragma once
#include"SceneBase.h"
#include<memory>//どうせ後々使うので入れておく
#include<d3d12.h>
#include"GameScene.h"
#include"ColorTriangle.h"
#include"DoubleTrigle.h"
#include"Particle.h"
#include"CameraController.h"

//クラスで関数を作ってから入れるのだ
class GameManager :public Scene {
	GameScene scene = GameScene::COLORTRIANGLE;
	ColorTriangle color_;
	DoubleTrigle dTri_;
	Particle particle_;
	CameraController camecon_;
	void GUI();
public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

	GameManager();
	~GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
};
