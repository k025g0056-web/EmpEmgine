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
    blockModel_ = EmpEngine::Resource().Model().LoadObjFile("teapot", "teapot.obj");
    blockHandle_ = EmpEngine::Resource().Texture().Load("Cube.png");
}

void GameManager::Update() {
 

    debugCamera_.Update();
    EmpEngine::LightGUI();
} 

void GameManager::Draw() {
    EmpEngine::Draw().DrawModel(blockModel_, transform_, blockHandle_, Vector4{ 1.0f,1.0f,1.0f,1.0f });
}