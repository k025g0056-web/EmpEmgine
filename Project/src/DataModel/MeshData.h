#pragma once
#include"VertexData.h"
#include<vector>
#include<string>
#include"Node.h"

struct MeshData {
	std::string objectName;
	std::string materialName;
	std::vector<VertexData> vertices;
	Node rootNode;
};