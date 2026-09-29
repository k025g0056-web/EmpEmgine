#include "Player.h"
#include"src/core/wrapper/EmpEngine.h"
#include"src/DataModel/Vector4.h"
#include"src/interaction/Input/Input.h"

void Player::Initialize(
    const ModelData& modelData,
    D3D12_GPU_DESCRIPTOR_HANDLE textureHandle,
    Vector3 startPosition
) {
    modelData_ = modelData;
    textureHandle_ = textureHandle;
    transform_.translate = startPosition;
    previousPosition_ = startPosition;
}

void Player::Update() {
    previousPosition_ = transform_.translate;
    if (Input::GetInstance()->IsRelease(DIK_SPACE)) {
        speed_ *= -1.0f;
    }
   

  
    //transform_.rotate.y += 0.02f;
}

void Player::Draw() {
    if (isAlive_) {
        EmpEngine::Draw().DrawModel(modelData_, transform_, textureHandle_, Vector4{ 1.0f,1.0f,1.0f,1.0f });
    }
}

