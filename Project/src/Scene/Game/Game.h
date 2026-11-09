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
#include"DataModel/Vector2.h"

class GameManager :public Scene {
	ModelData ItemModel_{};
	D3D12_GPU_DESCRIPTOR_HANDLE ItemTextureHandle_;
	ModelData blockModel_{};
	D3D12_GPU_DESCRIPTOR_HANDLE blockHandle_;
	ModelData goalModel_{};
	ModelData playerModel_{};
	D3D12_GPU_DESCRIPTOR_HANDLE playerHandle_;
	Transform3d transform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

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
};
