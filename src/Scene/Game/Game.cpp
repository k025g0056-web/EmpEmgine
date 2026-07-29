#include "Game.h"
#include <time.h>
#include <cmath>
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
    modelData_ = EmpEngine::LoadObjFile("resources/3dObject/axis", "axis.obj");
    Alarm01_ = EmpEngine::SoundLoadWave("fanfare.wav");
    ballStartPos_ = ballTransform_.translate;
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
    // ↓ DrawPreModel → DrawCompressModel に変更なの
    EmpEngine::DrawCompressModel(transform, sphereHandle, model_);
    DrawBall();
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
    ImGui::SliderFloat("weight (kg)", &weight, 1.0f, 300.0f);
    ImGui::Combo("easing", &easing,
        "Linear\0EaseIn\0EaseOut\0EaseInOut\0Bounce\0");

    if (ImGui::Button("Reset")) {
        model_.Initialize(EmpEngine::GetDevice(), modelData_);
        compressStarted_ = false;
    }

    ImGui::Text(model_.IsFinished() ? "完了！" : "圧縮中...");

    if (ImGui::Button("Start Ball"))
    {
        isBallMove_ = true;

        constexpr float startDistance = 6.0f;

        switch (ballAxis_)
        {
        case BallAxis::X:
            ballStartPos_ = {
                transform.translate.x - startDistance,
                transform.translate.y,
                transform.translate.z
            };
            ballVelocity_ = { 2.0f,0.0f,0.0f };
            break;

        case BallAxis::Y:
            ballStartPos_ = {
                transform.translate.x,
                transform.translate.y + startDistance,
                transform.translate.z
            };
            ballVelocity_ = { 0.0f,-2.0f,0.0f };
            break;

        case BallAxis::Z:
            ballStartPos_ = {
                transform.translate.x,
                transform.translate.y,
                transform.translate.z - startDistance
            };
            ballVelocity_ = { 0.0f,0.0f,2.0f };
            break;
        }

        ballTransform_.translate = ballStartPos_;
    }

    static int moveAxis = 0;

    ImGui::SliderFloat("HitDistance", &hitDistance_, 0.5f, 6.0f);

    ImGui::Combo("BallAxis", &moveAxis, "X\0Y\0Z\0");

    ballAxis_ = static_cast<BallAxis>(moveAxis);

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

    if (isBallMove_)
    {
        UpdateBall(deltaTime_);
        TryCompressOnHit();
    }
}

bool GameManager::CheckHitModel() const {
    const Vector3& ballPos = ballTransform_.translate;
    const Vector3& modelPos = transform.translate;

    float dx = ballPos.x - modelPos.x;
    float dy = ballPos.y - modelPos.y;
    float dz = ballPos.z - modelPos.z;
    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

    float modelRadius = transform.scale.x; // モデル側の当たり判定サイズ（近似。実寸に合わせて調整してください）

    return distance < hitDistance_;
}

void GameManager::TryCompressOnHit() {
    if (!CheckHitModel()) return;

    // 潰れてる最中に再ヒットしても多重発動させない
    if (model_.IsCompressing()) return;

    // 「たまに」の抽選（30%の確率で発動。数値はお好みで）
    constexpr int kHitChancePercent = 100;
    if (rand() % 100 >= kHitChancePercent) return;

    // 当たるたびに重さをランダムに変えると、潰れ方に毎回バリエーションが出る
   // float weight = 0.0f;//20.0f + static_cast<float>(rand() % 260); // 20〜280kg

    CompressAxis axis;

    switch (ballAxis_)
    {
    case BallAxis::X:
        axis = CompressAxis::X;
        break;

    case BallAxis::Y:
        axis = CompressAxis::Y;
        break;

    default:
        axis = CompressAxis::Z;
        break;
    }

    model_.StartCompress({
        axis,
        CompressEasing::Bounce,
        0.8f,
        0.0f,
        weight,
        });

    ballTransform_.translate = ballStartPos_;

    ballTransform_.translate = ballStartPos_;
    isBallMove_ = false;
}

void GameManager::UpdateBall(float dt) {
    switch (ballAxis_){
    case BallAxis::X:
        ballTransform_.translate.x += ballVelocity_.x * dt;
        if (ballTransform_.translate.x > 4 || ballTransform_.translate.x < -7)
            ballVelocity_.x *= -1;
        break;

    case BallAxis::Y:
        ballTransform_.translate.y += ballVelocity_.y * dt;
        if (ballTransform_.translate.y > 4 || ballTransform_.translate.y < -7)
            ballVelocity_.y *= -1;
        break;

    case BallAxis::Z:
        ballTransform_.translate.z += ballVelocity_.z * dt;
        if (ballTransform_.translate.z > 4 || ballTransform_.translate.z < -7)
            ballVelocity_.z *= -1;
        break;
    }
}

void GameManager::DrawBall() {
    EmpEngine::DrawSphere(ballTransform_, { 1.0f, 1.0f, 1.0f, 1.0f });
}