#include "Enclore.h"

void Enclosure::Initialize(ModelData modelData, D3D12_GPU_DESCRIPTOR_HANDLE texture) {
	modelData_ = modelData;
	texture_ = texture;
}



void Enclosure::Update() {
	if (isAllSet_) {
		//モデルの崩壊モーション
		for (int i = 0; i < 4; i++) {
			isSet_[i] = false;
		}

		isAllSet_ = false;
	}
}


void Enclosure::Draw() {
	if (isSet_[0]&& isSet_[1]) {

	}

	if (isSet_[1] && isSet_[2]) {

	}

	if (isSet_[2] && isSet_[3]) {

	}

	if (isSet_[3] && isSet_[0]) {

	}
}