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
    monsterBall = EmpEngine::Resource().Texture().Load("monsterBall.png");
    modelData_ = EmpEngine::Resource().Model().LoadObjFile("resources/3dObject/fence", "fence.obj");
    fence = EmpEngine::Resource().Texture().Load("fence.png");
    // ↓ 追加なの
    model_.Initialize(EmpEngine::GetDevice(), modelData_);
}

void GameManager::Update() {
    colorBuff = SprColor * 255.0f;
    // ↓ デルタタイム計算を追加なの
    static auto prev = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    deltaTime_ = std::chrono::duration<float>(now - prev).count();
    prev = now;

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
    ImGui::SliderAngle("Rotatex", &transform.rotate.x);
    ImGui::SliderAngle("Rotatey", &transform.rotate.y);
    ImGui::SliderAngle("Rotatez", &transform.rotate.z);
    ImGui::Checkbox("useMonsterBall", &useMonsterBall);

    ImGui::End();

    EmpEngine::LightGUI();
#endif
}

void GameManager::UpdateHomeWork() {
    if (Input::GetInstance()->IsReleaseVk(MDK_SPACE)) {
        if (useMonsterBall) {
            useMonsterBall = false;
        }
        else {
            useMonsterBall = true;
        }
    }

    if (useMonsterBall) {
        sphereHandle = monsterBall;
    }
    else {
        sphereHandle = uvChecker;
    }

    SprColor = colorBuff / 255.0f;
    debugCamera_.Update();
    SceneSystem::GetCamera()->SetCameraRotate(debugCamera_.GetRotate());
    SceneSystem::GetCamera()->SetCameraPosition(debugCamera_.GetTranslate());
    SceneSystem::GetCamera()->Update(windowWidth_, windowHeight_);

    // ↓ 追加なの
    model_.Update(deltaTime_);
}