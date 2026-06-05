#include"Game.h"
#include<time.h>
#include"EmpEngine.h"
#include"SceneSystem.h"
#include"Input.h"
#include"externals/imgui/imgui.h"


//先に初期化を入れておく
GameManager::GameManager() {
	srand(static_cast<unsigned int>(time(nullptr)));
}

void GameManager::Initialize() {
    color_.initialize();
    dTri_.Initialize();
    particle_.Initialize();
    camecon_.Initialize(SceneSystem::GetCamera()->GetCameraPosition(),SceneSystem::GetCamera()->GetCameraRotate());
}

void GameManager::Update() {
    GUI();
    if (scene==GameScene::PARTIClE) {
        EmpEngine::SetWindowColor({ 0.0f,0.0f,0.0f,1.0f });
    } else {
        EmpEngine::SetWindowColor({ 0.1f,0.25f,0.5f,1.0f });
    }

    camecon_.Update();
    SceneSystem::GetCamera()->SetCameraPosition(camecon_.GetTranslate());
    SceneSystem::GetCamera()->SetCameraRotate(camecon_.GetRotate());
	SceneSystem::GetCamera()->Update(windowWidth_, windowHeight_);
}

void GameManager::Draw() {
    switch (scene){
    case GameScene::COLORTRIANGLE:
        color_.Draw();
        break;
    case GameScene::DOUBLETRIANGLE:
        dTri_.Draw();
        break;
    case GameScene::PARTIClE:
        particle_.Draw();
        break;
    }
}

void GameManager::GUI() {
#ifdef USE_IMGUI
    if (ImGui::BeginTabBar("MyTabs")){
        if (ImGui::BeginTabItem("ChangeTriangleColor")){
            scene = GameScene::COLORTRIANGLE;
            color_.GUI();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("DoubleTriangle")){
            scene = GameScene::DOUBLETRIANGLE;
            dTri_.GUI();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Particle")){
            scene = GameScene::PARTIClE;
            particle_.GUI();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
	
    EmpEngine::LightGUI();

#endif 

}
