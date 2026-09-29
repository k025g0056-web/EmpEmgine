#include "Item.h"
#include"src/core/wrapper/EmpEngine.h"

void Item::Initialize(const ModelData& modelData, D3D12_GPU_DESCRIPTOR_HANDLE textureHandle, Typed type, Vector3 position) {
	modelData_ = modelData;
	textureHandle_ = textureHandle;
	type_ = type;
	transform_.translate = position;
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	switch (type_)
	{
	case Item::Typed::unKnown:
		break;
	case Item::Typed::Up:
		color_= { 1.0f, 0.0f, 1.0f, 1.0f };
		break;
	case Item::Typed::down:
		color_= { 0.0f, 1.0f, 1.0f, 1.0f };
		break;
	default:
		break;
	}
}

void Item::Update() {
	transform_.rotate.x += 0.01f;
	transform_.rotate.y += 0.01f;
}

void Item::Draw() {
	EmpEngine::Draw().DrawModel(modelData_, transform_, textureHandle_, color_);
}