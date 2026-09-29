#pragma once
#include"src/DataModel/TransForm3d.h"
#include"src/DataModel/ModelData.h"
#include"src/DataModel/Vector4.h"
#include<d3d12.h>

class Item{
public:
	enum class Typed {
		unKnown,
		Up,
		down
	};

	void Initialize(const ModelData& modelData, D3D12_GPU_DESCRIPTOR_HANDLE textureHandle, Typed type,Vector3 position);

	void Update();

	void Draw();

	void SetSpeed(float& speed) {

		if (isActive_) {
			switch (type_)
			{
			case Item::Typed::unKnown:
				break;
			case Item::Typed::Up:
				speed *= 1.2f;
				break;
			case Item::Typed::down:
				speed *= 0.9f;
				break;
			}
		}

	}

	Vector3 GetSize() const {
		return {
			0.794f * transform_.scale.x,
			0.873f * transform_.scale.y,
			0.132f * transform_.scale.z
		};
	}

	void Collect() {
		if (!isActive_) {
			return;
		}

		isActive_ = false;
		color_.w = 0.0f;
	}

	Vector3 GetTranslate() const {
		return transform_.translate;
	}

	bool GetIsActive() const {
		return isActive_;
	}

private:
	Transform3d transform_{ {0.9f,0.9f,0.9f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	ModelData modelData_{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureHandle_{};
	Typed type_ = Typed::unKnown;
	bool isActive_ = true;
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

};

