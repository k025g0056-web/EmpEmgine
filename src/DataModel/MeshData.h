#pragma once
#pragma once
#include"VertexData.h"
#include<vector>
#include<string>

struct MeshData {
	std::string materialName;
	std::vector<VertexData> vertices;
};