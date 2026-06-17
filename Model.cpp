#include "Model.h"
#include"DX12Mechanics.h"

void Model::Initialize(ID3D12Device* device,const ModelData& modelData) {
	Shape::Initialize(device, true);//親が先やでん
	modelData_ = modelData;
	vertexResource = DX12Mechanics::CreateBufferResource(device, sizeof(VertexData) * modelData_.vertices.size());
	vertexBufferView_ = DX12Mechanics::GenerateVertexBufferView<VertexData>(vertexResource, modelData_.vertices.size());
}

