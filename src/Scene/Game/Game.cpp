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

    //スザンヌ
    suzanne_.modelData= EmpEngine::Resource().Model().LoadObjFile("resources/3dObject/suzanne", "suzanne.obj");
    suzanne_.model.Initialize(EmpEngine::GetDevice(), suzanne_.modelData);

    //うさぎ
    bunny_.modelData= EmpEngine::Resource().Model().LoadObjFile("resources/3dObject/bunny", "bunny.obj");
    bunny_.model.Initialize(EmpEngine::GetDevice(), bunny_.modelData);

    //ポット
    teaPot_.modelData= EmpEngine::Resource().Model().LoadObjFile("resources/3dObject/teapot", "teapot.obj");
    teaPot_.model.Initialize(EmpEngine::GetDevice(), teaPot_.modelData);

    fanfare_ = EmpEngine::Resource().Audio().SoundLoadWave();
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
    EmpEngine::DrawTextureSphere(Sphere_.transform, { 1.0f,1.0f,1.0f,1.0f }, uvChecker_);
    EmpEngine::DrawCompressModel(teaPot_.transform, checkerBoard_, teaPot_.model);
    EmpEngine::DrawCompressModel(bunny_.transform, uvChecker_, bunny_.model);
    EmpEngine::DrawCompressModel(suzanne_.transform, EmpEngine::GetWhite1x1(), suzanne_.model);
    EmpEngine::DrawSprite(transformSpr, { 0.0f,360.0f }, { 0.0f,0.0f }, { 640.0f,360.0f }, { 640.0f,0.0f }, SprColor, uvChecker_, uvTransformSpr);
}

void GameManager::GuiHomeWork() {
#ifdef USE_IMGUI
    ImGui::Begin("Homework");
    if (ImGui::Button("playSound")) {

   }

    ImGui::End();

    ImGui::Begin("suzanne");
    ImGui::SliderFloat3("scale", &suzanne_.transform.scale.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("rotate", &suzanne_.transform.rotate.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("translate", &suzanne_.transform.translate.x, -3.0f, 3.0f);

    ImGui::End();

    ImGui::Begin("bunny");
    ImGui::SliderFloat3("scale", &bunny_.transform.scale.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("rotate", &bunny_.transform.rotate.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("translate", &bunny_.transform.translate.x, -3.0f, 3.0f);

    ImGui::End();

    ImGui::Begin("teaPot");
    ImGui::SliderFloat3("scale", &teaPot_.transform.scale.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("rotate", &teaPot_.transform.rotate.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("translate", &teaPot_.transform.translate.x, -3.0f, 3.0f);

    ImGui::End();

    ImGui::Begin("Sphere");
    ImGui::SliderFloat3("scale", &Sphere_.transform.scale.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("rotate", &Sphere_.transform.rotate.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("translate", &Sphere_.transform.translate.x, -3.0f, 3.0f);

    ImGui::End();


    ImGui::Begin("Sprite");
    ImGui::SliderFloat3("scale", &transformSpr.scale.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("rotate", &transformSpr.rotate.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("translate", &transformSpr.translate.x, -3.0f, 3.0f);

    ImGui::BeginChild("UV");
    ImGui::SliderFloat3("uvscale", &uvTransformSpr.scale.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("uvrotate", &uvTransformSpr.rotate.x, -3.0f, 3.0f);
    ImGui::SliderFloat3("uvtranslate", &uvTransformSpr.translate.x, -3.0f, 3.0f);

    ImGui::EndChild();
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