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

    modelDataLoaded_.fill(false);
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

// Sphere〜Multi_Materialの.objを初回だけ読み込む
const ModelData& GameManager::GetOrLoadModelData(Models modelType) {
    // Sphere=1 なので配列インデックスは -1 して0始まりにする
    int cacheIndex = static_cast<int>(modelType) - static_cast<int>(Sphere);

    if (!modelDataLoaded_[cacheIndex]) {
        const ModelPath& path = modelPaths_[cacheIndex];
        modelDataCache_[cacheIndex] = EmpEngine::Resource().Model().LoadObjFile(path.directory, path.filename);
        modelDataLoaded_[cacheIndex] = true;
    }
    return modelDataCache_[cacheIndex];
}

// 選択中のモデル種別に応じて新規に1体追加する
void GameManager::CreateSelectedModel() {
    switch (models) {
    case Sprite: {
        SpritePar newSprite;
        newSprite.textureHandle = uvChecker_;
        sprite_.push_back(newSprite);
        break;
    }
    case Sphere: {
        OBJ newObj;
        newObj.modelData = GetOrLoadModelData(Sphere);
        newObj.model.Initialize(EmpEngine::GetDevice(), newObj.modelData);
        sphere_.push_back(std::move(newObj));
        break;
    }
    case Stanford_Bunny: {
        OBJ newObj;
        newObj.modelData = GetOrLoadModelData(Stanford_Bunny);
        newObj.model.Initialize(EmpEngine::GetDevice(), newObj.modelData);
        stanfordBunny_.push_back(std::move(newObj));
        break;
    }
    case Plane: {
        OBJ newObj;
        newObj.modelData = GetOrLoadModelData(Plane);
        newObj.model.Initialize(EmpEngine::GetDevice(), newObj.modelData);
        plane_.push_back(std::move(newObj));
        break;
    }
    case UtahTeapot: {
        OBJ newObj;
        newObj.modelData = GetOrLoadModelData(UtahTeapot);
        newObj.model.Initialize(EmpEngine::GetDevice(), newObj.modelData);
        utahTeaPot_.push_back(std::move(newObj));
        break;
    }
    case Multi_Mesh: {
        OBJ newObj;
        newObj.modelData = GetOrLoadModelData(Multi_Mesh);
        newObj.model.Initialize(EmpEngine::GetDevice(), newObj.modelData);
        multiMesh_.push_back(std::move(newObj));
        break;
    }
    case Multi_Material: {
        OBJ newObj;
        newObj.modelData = GetOrLoadModelData(Multi_Material);
        newObj.model.Initialize(EmpEngine::GetDevice(), newObj.modelData);
        multiMaterial_.push_back(std::move(newObj));
        break;
    }
    }
}

void GameManager::DrawHomeWork() {
    D3D12_GPU_DESCRIPTOR_HANDLE white = EmpEngine::GetWhite1x1();

    for (auto& obj : suzanne_) {
        EmpEngine::DrawCompressModel(obj.transform, white, obj.model);
    }
    for (auto& obj : sphere_) {
        EmpEngine::DrawCompressModel(obj.transform, white, obj.model);
    }
    for (auto& obj : stanfordBunny_) {
        EmpEngine::DrawCompressModel(obj.transform, white, obj.model);
    }
    for (auto& obj : plane_) {
        EmpEngine::DrawCompressModel(obj.transform, white, obj.model);
    }
    for (auto& obj : utahTeaPot_) {
        EmpEngine::DrawCompressModel(obj.transform, white, obj.model);
    }
    for (auto& obj : multiMesh_) {
        EmpEngine::DrawCompressModel(obj.transform, white, obj.model);
    }
    for (auto& obj : multiMaterial_) {
        EmpEngine::DrawCompressModel(obj.transform, white, obj.model);
    }

    for (auto& sprite : sprite_) {
        EmpEngine::DrawSprite(sprite.transform, sprite.point0, sprite.point1, sprite.point2, sprite.point3,
            sprite.color, sprite.textureHandle, sprite.uvTransform);
    }
}

// OBJ1体分のトランスフォームをImGuiで編集
// 戻り値: 削除ボタンが押されたらtrue
bool GameManager::EditObjGui(const char* label, int index, OBJ& obj) {
    bool removeRequested = false;

    ImGui::PushID(index);
    char nodeLabel[64];
    sprintf_s(nodeLabel, "%s [%d]", label, index);

    if (ImGui::TreeNode(nodeLabel)) {
        ImGui::DragFloat3("Scale", &obj.transform.scale.x, 0.01f);
        ImGui::DragFloat3("Rotate", &obj.transform.rotate.x, 0.01f);
        ImGui::DragFloat3("Translate", &obj.transform.translate.x, 0.05f);
        ImGui::ColorEdit4("Color", &obj.color.x);

        if (ImGui::Button("Remove")) {
            removeRequested = true;
        }

        ImGui::TreePop();
    }
    ImGui::PopID();

    return removeRequested;
}

// Sprite1体分のトランスフォーム・UVトランスフォームを編集
bool GameManager::EditSpriteGui(int index, SpritePar& sprite) {
    bool removeRequested = false;

    ImGui::PushID(index);
    char nodeLabel[64];
    sprintf_s(nodeLabel, "Sprite [%d]", index);

    if (ImGui::TreeNode(nodeLabel)) {
        ImGui::Text("Transform");
        ImGui::DragFloat3("Scale##T", &sprite.transform.scale.x, 0.01f);
        ImGui::DragFloat3("Rotate##T", &sprite.transform.rotate.x, 0.01f);
        ImGui::DragFloat3("Translate##T", &sprite.transform.translate.x, 0.5f);

        ImGui::Separator();
        ImGui::Text("UV Transform");
        ImGui::DragFloat3("Scale##UV", &sprite.uvTransform.scale.x, 0.01f);
        ImGui::DragFloat3("Rotate##UV", &sprite.uvTransform.rotate.x, 0.01f);
        ImGui::DragFloat3("Translate##UV", &sprite.uvTransform.translate.x, 0.01f);

        ImGui::Separator();
        ImGui::ColorEdit4("Color", &sprite.color.x);

        if (ImGui::Button("Remove")) {
            removeRequested = true;
        }

        ImGui::TreePop();
    }
    ImGui::PopID();

    return removeRequested;
}

// vector<OBJ>を一覧表示・編集し、削除要求があればその場で消す
void GameManager::DrawObjListGui(const char* categoryLabel, std::vector<OBJ>& objList) {
    if (objList.empty()) return;

    if (ImGui::CollapsingHeader(categoryLabel)) {
        for (int i = 0; i < static_cast<int>(objList.size()); ++i) {
            if (EditObjGui(categoryLabel, i, objList[i])) {
                objList.erase(objList.begin() + i);
                --i; // 削除した分インデックスを戻す
            }
        }
    }
}

void GameManager::GuiHomeWork() {
#ifdef USE_IMGUI
    ImGui::Begin("Homework");

    if (ImGui::Combo("Model", &modelIndex_, modelTable, IM_ARRAYSIZE(modelTable))) {
        models = static_cast<Models>(modelIndex_);
    }

    if (ImGui::Button("Create")) {
        CreateSelectedModel();
    }

    ImGui::Separator();

    // Sprite一覧
    if (!sprite_.empty() && ImGui::CollapsingHeader("Sprite")) {
        for (int i = 0; i < static_cast<int>(sprite_.size()); ++i) {
            if (EditSpriteGui(i, sprite_[i])) {
                sprite_.erase(sprite_.begin() + i);
                --i;
            }
        }
    }

    // モデル一覧
    DrawObjListGui("Sphere", sphere_);
    DrawObjListGui("Stanford_Bunny", stanfordBunny_);
    DrawObjListGui("Plane", plane_);
    DrawObjListGui("UtahTeapot", utahTeaPot_);
    DrawObjListGui("Multi_Mesh", multiMesh_);
    DrawObjListGui("Multi_Material", multiMaterial_);
    DrawObjListGui("Suzanne", suzanne_);

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