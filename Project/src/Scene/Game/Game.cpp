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
    plane_ = EmpEngine::Resource().Model().LoadObjFile("plane", "plane.obj");
    myCharactorHandle_ = EmpEngine::Resource().Texture().Load("circle.png");
    particle_.Initialize(parthicleCount);
}
                       
void GameManager::Update() {
    particle_.Update();
    debugCamera_.Update();
    SceneSystem::GetCamera()->SetCameraPosition(debugCamera_.GetTranslate());
    SceneSystem::GetCamera()->SetCameraRotate(debugCamera_.GetRotate());
   
    EmpEngine::LightGUI();
} 

void GameManager::Draw() {
    particle_.Draw(plane_,myCharactorHandle_);
    EmpEngine::DrawLight();
}