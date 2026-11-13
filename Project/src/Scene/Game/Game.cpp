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
    GUI();
    GeneParticle();
    debugCamera_.Update();
    SceneSystem::GetCamera()->SetCameraPosition(debugCamera_.GetTranslate());
    SceneSystem::GetCamera()->SetCameraRotate(debugCamera_.GetRotate());
   
    EmpEngine::LightGUI();
} 

void GameManager::Draw() {
    EmpEngine::Draw().DrawModel<Particle3D>(blockModel_, transform_,blockHandle_, Vector4{1.0f,1.0f,1.0f,1.0f},parthicleCount);
    EmpEngine::DrawLight();
}

void GameManager::GUI() {
#ifdef USE_IMGUI
    ImGui::Begin("Particle");
    ImGui::SliderInt("Count", &parthicleCount, 1, 100000);
    ImGui::End();
#endif // USE_IMGUI
}

void GameManager::GeneParticle() {

    int count = parthicleCount - static_cast<int>(transform_.size());

    for (int i = 0; i < parthicleCount;i++) {
        transform_[i].scale = { 1.0f,1.0f,1.0f };
        transform_[i].rotate = { 0.0f,0.0f,0.0f };
        transform_[i].translate = { i * 0.1f,i * 0.1f,i * 0.1f };
    }
}