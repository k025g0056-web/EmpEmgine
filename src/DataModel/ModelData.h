#pragma once
#include"MeshData.h"
#include<vector>
#include"MaterialData.h"

struct ModelData{
	std::vector<MeshData> meshes;
	std::vector<MaterialData>materials;
};