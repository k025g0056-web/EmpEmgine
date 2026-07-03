#include "Game.h"
#include <time.h>
#include "core/wrapper/EmpEngine.h"
#include "Scene/wrapper/SceneSystem.h"
#include "Intraction/Input/Input.h"
#include "externals/imgui/imgui.h"
#include <chrono> // ← 追加なの

GameManager::GameManager() {
    srand(static_cast<unsigned int>(time(nullptr)));
}

void GameManager::Initialize() {
    debugCamera_.Initialize(
        SceneSystem::GetCamera()->GetCameraPosition(),
        SceneSystem::GetCamera()->GetCameraRotate());

    uvChecker = EmpEngine::LoadTexture("uvChecker.png");
    monsterBall = EmpEngine::LoadTexture("monsterBall.png");
    modelData_ = EmpEngine::LoadObjFile("resources/3dObject/bunny", "bunny.obj");
    Alarm01_ = EmpEngine::SoundLoadWave("fanfare.wav");

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

    if (Input::GetInstance()->IsTriggerVk(MDK_ENTER)) {
        EmpEngine::PlayAudio(Alarm01_);
    }


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
    // ↓ DrawPreModel → DrawCompressModel に変更なの
    EmpEngine::DrawCompressModel(transform, sphereHandle, model_);
    EmpEngine::DrawSphere(transform, { 1.0f,1.0f,1.0f,1.0f });
}

void GameManager::GuiHomeWork() {
#ifdef USE_IMGUI
    ImGui::Begin("Homework");
    ImGui::SliderAngle("Rotatex", &transform.rotate.x);
    ImGui::SliderAngle("Rotatey", &transform.rotate.y);
    ImGui::SliderAngle("Rotatez", &transform.rotate.z);
    ImGui::Checkbox("useMonsterBall", &useMonsterBall);
    EmpEngine::LightGUI();

    // ↓ 追加なの
    ImGui::Separator();
    ImGui::Text("-- Compress --");

    static float duration = 1.5f;
    static float delay = 0.0f;
    static int   axis = 1; // 0=X 1=Y 2=Z
    static int   easing = 0; // 0=Linear 1=EaseIn ...

    ImGui::SliderFloat("duration", &duration, 0.1f, 3.0f);
    ImGui::SliderFloat("delay", &delay, 0.0f, 2.0f);
    ImGui::Combo("axis", &axis, "X\0Y\0Z\0");
    ImGui::Combo("easing", &easing,
        "Linear\0EaseIn\0EaseOut\0EaseInOut\0Bounce\0");

    if (ImGui::Button("Start Compress!")) {
        model_.StartCompress({
            static_cast<CompressAxis>(axis),
            static_cast<CompressEasing>(easing),
            duration,
            delay,
            });
        compressStarted_ = true;
    }

    if (ImGui::Button("Reset")) {
        model_.Initialize(EmpEngine::GetDevice(), modelData_);
        compressStarted_ = false;
    }

    ImGui::Text(model_.IsFinished() ? "完了！" : "圧縮中...");

    ImGui::End();
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