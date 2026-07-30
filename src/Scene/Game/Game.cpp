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

    uvChecker = EmpEngine::Resource().Texture().Load("uvChecker.png");
    modelData_ = EmpEngine::Resource().Model().LoadObjFile("resources/3dObject/suzanne", "suzanne.obj");
    fence = EmpEngine::Resource().Texture().Load("fence.png");

    model_.Initialize(EmpEngine::GetDevice(), modelData_);
}

void GameManager::Update() {
    colorBuff = SprColor * 255.0f;

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
    
    EmpEngine::DrawCompressModel(transform, fence, model_);
    
}

void GameManager::GuiHomeWork() {
#ifdef USE_IMGUI
    ImGui::Begin("Homework");
   
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