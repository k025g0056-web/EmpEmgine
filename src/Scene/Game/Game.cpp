#include "Game.h"
#include <time.h>
#include "core/wrapper/EmpEngine.h"
#include "Scene/wrapper/SceneSystem.h"
#include "Interaction/Input/Input.h"
#include "externals/imgui/imgui.h"
#include <chrono> 

GameManager::GameManager() {
    srand(static_cast<unsigned int>(time(nullptr)));
}

void GameManager::Initialize() {
    debugCamera_.Initialize(
        SceneSystem::GetCamera()->GetCameraPosition(),
        SceneSystem::GetCamera()->GetCameraRotate());

    uvChecker_ = EmpEngine::Resource().Texture().Load("uvChecker.png");
    monsterBall_ = EmpEngine::Resource().Texture().Load("monsterBall.png");
    checkerBoard_ = EmpEngine::Resource().Texture().Load("checkerBoard.png");

    //Sprite
    sprite_.textureHandle = uvChecker_;

    //スザンヌ
    suzanne_.modelData = EmpEngine::Resource().Model().LoadObjFile("resources/3dObject/suzanne", "suzanne.obj");
    suzanne_.model.Initialize(EmpEngine::GetDevice(), suzanne_.modelData);
}

void GameManager::Update() {

    GuiHomeWork();
    UpdateHomeWork();
}

void GameManager::Draw() {
    DrawHomeWork();
    EmpEngine::DrawLight();
}

void GameManager::GUI() {
#ifdef USE_IMGUI
#endif
}

void GameManager::DrawHomeWork() {
    EmpEngine::DrawCompressModel(suzanne_.transform, EmpEngine::GetWhite1x1(), suzanne_.model);
    EmpEngine::DrawSprite(sprite_.transform, sprite_.point0, sprite_.point1, sprite_.point2, sprite_.point3, sprite_.color, sprite_.textureHandle, sprite_.uvTransform);
}

void GameManager::GuiHomeWork() {
#ifdef USE_IMGUI
    ImGui::Begin("Homework");
    if (ImGui::Combo("Model", &modelIndex_, modelTable, IM_ARRAYSIZE(modelTable))) {
            models = static_cast<Models>(modelIndex_);
    }

    if (ImGui::Button("Create")) {
        switch (models) {
        case GameManager::Sprite:
            break;
        case GameManager::Sphere:
            break;
        case GameManager::Stanford_Bunny:
            break;
        case GameManager::Plane:
            break;
        case GameManager::UtahTeapot:
            break;
        case GameManager::Multi_Mesh:
            break;
        case GameManager::Multi_Material:
            break;
        }
    }

    switch (models){
    case GameManager::Sprite:
        break;
    case GameManager::Sphere:
        break;
    case GameManager::Stanford_Bunny:
        break;
    case GameManager::Plane:
        break;
    case GameManager::UtahTeapot:
        break;
    case GameManager::Multi_Mesh:
        break;
    case GameManager::Multi_Material:
        break;
    }


    ImGui::End();

    EmpEngine::LightGUI();
#endif
}

void GameManager::UpdateHomeWork() {
    
    debugCamera_.Update();
    SceneSystem::GetCamera()->SetCameraRotate(debugCamera_.GetRotate());
    SceneSystem::GetCamera()->SetCameraPosition(debugCamera_.GetTranslate());
    SceneSystem::GetCamera()->Update(windowWidth_, windowHeight_);
}