#include "MapManager.h"
#include"src/core/wrapper/EmpEngine.h"
#include<numbers>
#include"Collision.h"
#include <cmath>

void MapManager::Initialize(ModelData itemModel, D3D12_GPU_DESCRIPTOR_HANDLE itemHandle,
    ModelData blockModel, D3D12_GPU_DESCRIPTOR_HANDLE blockHandle,
    ModelData goalModel, D3D12_GPU_DESCRIPTOR_HANDLE goalHandle) {
    itemModel_ = itemModel;
    itemHandle_ = itemHandle;
    blockModel_ = blockModel;
    blockHandle_ = blockHandle;
    goalModel_ = goalModel;
    goalHandle_ = goalHandle;

    int map[22][22] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,1,1,0,0,0,0,0,0,0,3,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,2,0,0,3,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,1},
    {1,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,1,0,0,1,1,0,0,0,0,1,0,0,0,1},
    {1,0,1,0,0,1,0,0,1,4,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
    {1,2,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,2,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1},
    {1,0,3,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,3,0,1},
    {1,0,0,0,0,0,0,0,3,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,2,0,0,0,0,0,0,2,0,0,0,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1},
    {1,0,5,0,0,0,0,2,0,3,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int i = 0; i < 22;i++) {
        for (int j = 0; j < 22;j++) {
            map_[i][j] = map[i][j];
            switch (map_[i][j]){
            case 1:
                block_[i][j].translate= { j * kSize, (21 - i) * kSize,0.0f };
                block_[i][j].scale= { 1.0f,1.0f,1.0f };
                break;
            case 2:
                itemUpPosition_[i][j]= { j * kSize, (21 - i) * kSize,0.0f };
                item_[i][j].Initialize(itemModel_, itemHandle_, Item::Typed::Up, itemUpPosition_[i][j]);
                break;
            case 3:
                itemDownPosition_[i][j]= { j * kSize, (21 - i) * kSize,0.0f };
                item_[i][j].Initialize(itemModel_, itemHandle_, Item::Typed::down, itemDownPosition_[i][j]);
                break;
            case 4:
                goal_.translate= { j * kSize, (21 - i) * kSize,0.0f };
                goal_.scale = { 1.0f,1.0f,1.0f };
                goal_.rotate.y = std::numbers::pi_v<float>;
                break;
            case 5:
                start_ = { j * kSize, (21 - i) * kSize,0.0f };
                break;
            }
        }
    }

}

void MapManager::Update() {
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            if (map_[i][j] == 2 || map_[i][j] == 3) {
                item_[i][j].Update();
            }
        }
    }
}

bool MapManager::CheckCollision(Player& player) {
    CheckItemCollision(player);
    return CheckGoalCollision(player);
}

void MapManager::CheckBlockCollisionX(Player& player) {
    Vector3 pos = player.GetTranslate();
    const Vector3 previousPos = player.GetPreviousTranslate();

    const Vector3 playerHalf = player.GetSize();
    const Vector3 blockHalf = {
        kSize / 2.0f,
        kSize / 2.0f,
        kSize / 2.0f
    };

    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            if (map_[i][j] != 1) {
                continue;
            }

            const Vector3 blockPos = block_[i][j].translate;

            const bool overlapX =
                std::abs(pos.x - blockPos.x) < playerHalf.x + blockHalf.x;
            const bool overlapY =
                std::abs(pos.y - blockPos.y) < playerHalf.y + blockHalf.y;

            if (overlapX && overlapY) {
                // X移動だけを取り消す
                pos.x = previousPos.x;
                player.SetTranslate(pos);
                return;
            }
        }
    }

    player.SetTranslate(pos);
}
void MapManager::CheckBlockCollisionY(Player& player) {
    Vector3 pos = player.GetTranslate();
    const Vector3 previousPos = player.GetPreviousTranslate();

    const Vector3 playerHalf = player.GetSize();
    const Vector3 blockHalf = {
        kSize / 2.0f,
        kSize / 2.0f,
        kSize / 2.0f
    };

    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            if (map_[i][j] != 1) {
                continue;
            }

            const Vector3 blockPos = block_[i][j].translate;

            const bool overlapX =
                std::abs(pos.x - blockPos.x) < playerHalf.x + blockHalf.x;
            const bool overlapY =
                std::abs(pos.y - blockPos.y) < playerHalf.y + blockHalf.y;

            if (overlapX && overlapY) {
                // Y移動だけを取り消す
                pos.y = previousPos.y;
                player.SetTranslate(pos);
                return;
            }
        }
    }

    player.SetTranslate(pos);
}void MapManager::CheckItemCollision(Player& player) {
    Vector3 pos = player.GetTranslate();
    Vector3 halfSize = player.GetSize();
    float speed = player.GetSpeed();

    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            if (map_[i][j] != 2 && map_[i][j] != 3) {
                continue;
            }
            if (!item_[i][j].GetIsActive()) {
                continue;
            }

            Vector3 itemPos = item_[i][j].GetTranslate();
            Vector3 itemHalf = item_[i][j].GetSize();

            if (CheckAABBCollision(pos, halfSize, itemPos, itemHalf)) {
                item_[i][j].SetSpeed(speed);
                item_[i][j].Collect();
            }
        }
    }

    player.SetSpeed(speed);
}

bool MapManager::CheckGoalCollision(Player& player) {
    Vector3 pos = player.GetTranslate();
    Vector3 halfSize = player.GetSize();
    
    Vector3 goalHalf = {
        0.715f * goal_.scale.x,
        1.054f * goal_.scale.y,
        0.399f * goal_.scale.z
    };

    return CheckAABBCollision(pos, halfSize, goal_.translate, goalHalf);
}


void MapManager::Draw() {
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            switch (map_[i][j]) {
            case 1:
                EmpEngine::Draw().DrawModel(blockModel_, block_[i][j], blockHandle_, Vector4{ 1.0f,1.0f,1.0f,1.0f });
                break;
            case 2:
            case 3:
                item_[i][j].Draw();
                break;
            case 4:
                EmpEngine::Draw().DrawModel(goalModel_, goal_, goalHandle_, Vector4{ 1.0f,1.0f,0.0f,1.0f });
                break;
            }
        }
    }
}
