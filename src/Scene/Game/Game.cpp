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
    blockModel_ = EmpEngine::Resource().Model().LoadObjFile("plane", "plane.obj");
    blockHandle_ = EmpEngine::Resource().Texture().Load("oirano.jpg");
}

void GameManager::Update() {
 

    debugCamera_.Update();
    SceneSystem::GetCamera()->SetCameraPosition(debugCamera_.GetTranslate());
    SceneSystem::GetCamera()->SetCameraRotate(debugCamera_.GetRotate());
    SceneSystem::GetCamera()->Update(windowWidth_, windowHeight_);
    EmpEngine::LightGUI();
} 

void GameManager::Draw() {
    EmpEngine::Draw().DrawModel(blockModel_, transform_,blockHandle_, Vector4{1.0f,1.0f,1.0f,1.0f});
    EmpEngine::DrawLight();
}