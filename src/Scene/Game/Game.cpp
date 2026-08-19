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
    EmpEngine::DrawTextureSphere(transform, { 1.0f,1.0f,1.0f,1.0f }, uvChecker_);
}

void GameManager::GuiHomeWork() {
#ifdef USE_IMGUI
    ImGui::Begin("Homework");
    if (ImGui::Button("playSound")) {
        //EmpEngine::Resource().Audio().SoundPlayWave(fanfare_);
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