#define NOMINMAX
#include "Game.h"
#include <time.h>
#include "core/wrapper/EmpEngine.h"
#include "Scene/wrapper/SceneSystem.h"
#include "Interaction/Input/Input.h"
#include "externals/imgui/imgui.h"
#include<algorithm>


GameManager::GameManager() {
    srand(static_cast<unsigned int>(time(nullptr)));
}

void GameManager::Initialize() {
    debugCamera_.Initialize(
        SceneSystem::GetCamera()->GetCameraPosition(),
        SceneSystem::GetCamera()->GetCameraRotate());
    ItemModel_ = EmpEngine::Resource().Model().LoadObjFile("Item", "Item.obj");
    ItemTextureHandle_ = EmpEngine::Resource().Texture().Load("Item.png");
    blockModel_ = EmpEngine::Resource().Model().LoadObjFile("Block", "Block.obj");
    blockHandle_ = EmpEngine::Resource().Texture().Load("Cube.png");
    goalModel_ = EmpEngine::Resource().Model().LoadObjFile("goal", "goal.obj");
    playerModel_ = EmpEngine::Resource().Model().LoadObjFile("Player", "player.obj");
    playerHandle_ = EmpEngine::Resource().Texture().Load("player.png");
    titleModel_ = EmpEngine::Resource().Model().LoadObjFile("plane", "plane.obj");

  
}

void GameManager::Update() {
    bool isGoal = false;

 switch (count){
   
    case 0:
        if (Input::GetInstance()->IsRelease(DIK_SPACE)) {
            count = 1;
            mapManager_.Initialize(
                ItemModel_, ItemTextureHandle_,
                blockModel_, blockHandle_,
                goalModel_, EmpEngine::GetWhite1x1());
            player_.Initialize(playerModel_, playerHandle_, mapManager_.GetStartPos());
        }
        SceneSystem::GetCamera()->SetCameraPosition({ titleTransform_.translate.x,titleTransform_.translate.y+10,titleTransform_.translate.z - 10.0f });
        break;
    case 1:
        mapManager_.Update();

        player_.Update();  // 入力・回転だけ

        player_.MoveX();
        mapManager_.CheckBlockCollisionX(player_);

        player_.MoveY();
        mapManager_.CheckBlockCollisionY(player_);

        isGoal = mapManager_.CheckCollision(player_);

        if (isGoal) {
            count = 2;
        }

        CameraPos_.x = std::clamp(player_.GetTranslate().x, 19.0f, 23.0f);
        CameraPos_.y = std::clamp(player_.GetTranslate().y, 11.0f, 31.0f);
        CameraPos_.z = std::clamp(player_.GetTranslate().z, -50.0f, -50.0f);

        SceneSystem::GetCamera()->SetCameraPosition(CameraPos_);


        break;
    case 2:
        if (Input::GetInstance()->IsRelease(DIK_SPACE)) {
            count = 0;
          
   
        }
        break;
    }



  
    SceneSystem::GetCamera()->Update(windowWidth_, windowHeight_);
}

void GameManager::Draw() {
    
    switch (count) {

    case 0:
       break;
    case 1:
        mapManager_.Draw();
        player_.Draw();
        break;
    case 2:
      
        break;
    }
    EmpEngine::Draw().DrawModel(titleModel_, titleTransform_, EmpEngine::GetWhite1x1(), Vector4{ 1.0f,1.0f,1.0f,1.0f });

}