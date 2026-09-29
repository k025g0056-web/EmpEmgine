#pragma once
#include"src/DataModel/TransForm3d.h"
#include"src/DataModel/Vector3.h"
#include"src/DataModel/ModelData.h"
#include<d3d12.h>

class Player {
    Transform3d transform_ = {
        {1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    };

    float speed_ = 0.06f;
    ModelData modelData_{};
    Vector3 size_ = { 1.1f, 1.0f, 0.832f };
    int hitPoint_ = 3;
    bool isAlive_ = true;

    D3D12_GPU_DESCRIPTOR_HANDLE textureHandle_{};
    Vector3 previousPosition_ = { 0.0f, 0.0f, 0.0f };

 public:
    void Initialize(
        const ModelData& modelData,
        D3D12_GPU_DESCRIPTOR_HANDLE textureHandle,
        Vector3 startPosition
    );

    void Update();
    void Draw();

    bool GetIsAlive() const {
        return isAlive_;
    }

    void Damage() {
        if (!isAlive_) {
            return;
        }

        hitPoint_--;

        if (hitPoint_ <= 0) {
            hitPoint_ = 0;
            isAlive_ = false;
        }
    }

    void Heal() {
        if (hitPoint_ < 3) {
            hitPoint_++;
        }
    }

    void MoveX() {
        transform_.translate.x += speed_;
    }

    void MoveY() {
        transform_.translate.y += speed_;
    }

    Vector3 GetTranslate() const { return transform_.translate; }
    Vector3 GetRotate() const { return transform_.rotate; }
    Vector3 GetScale() const { return transform_.scale; }
    float GetSpeed() const { return speed_; }
    Vector3 GetSize() const { return size_; }
    Vector3 GetPreviousTranslate() const { return previousPosition_; }

    void SetSize(Vector3 size) { size_ = size; }
    void SetTranslate(Vector3 translate) { transform_.translate = translate; }
    void SetRotate(Vector3 rotate) { transform_.rotate = rotate; }
    void SetScale(Vector3 scale) { transform_.scale = scale; }
    void SetSpeed(float speed) { speed_ = speed; }
};
