#pragma once
#include "src/DataModel/Vector3.h"
#include"src/DataModel/TransForm3d.h"
#include"src/DataModel/ModelData.h"
#include"Item.h"
#include"Player.h"
#include<d3d12.h>

class MapManager {
    int map_[22][22]{};
    Transform3d block_[22][22]{};//1
    Vector3 itemUpPosition_[22][22]{};//2
    Vector3 itemDownPosition_[22][22]{};//3
    Transform3d goal_{};//4
    float kSize = 2.0f;
    Vector3 start_{};

    Item item_[22][22]{};
    ModelData itemModel_;
    ModelData blockModel_;
    ModelData goalModel_;

    D3D12_GPU_DESCRIPTOR_HANDLE itemHandle_;
    D3D12_GPU_DESCRIPTOR_HANDLE blockHandle_;
    D3D12_GPU_DESCRIPTOR_HANDLE goalHandle_;

    void CheckBlockCollision(Player& player);
    void CheckItemCollision(Player& player);
    bool CheckGoalCollision(Player& player);
  

public:
    void Initialize(
        ModelData itemModel, D3D12_GPU_DESCRIPTOR_HANDLE itemHandle,
        ModelData blockModel, D3D12_GPU_DESCRIPTOR_HANDLE blockHandle,
        ModelData goalModel, D3D12_GPU_DESCRIPTOR_HANDLE goalHandle);
    void Update();
    void Draw();

    Vector3 GetStartPos() { return start_; }
   
    bool CheckCollision(Player& player);
    void CheckBlockCollisionX(Player& player);
    void CheckBlockCollisionY(Player& player);
};