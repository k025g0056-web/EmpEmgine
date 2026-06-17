#pragma once
#include "Shape.h"
#include"ModelData.h"
class Model :public Shape{
	ModelData modelData_;
public:
	void Initialize(ID3D12Device* device,const ModelData& modelData);
	void DrawModel()
};

