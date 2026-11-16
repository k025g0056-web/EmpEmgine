#pragma once
#include"DataModel/ModelData.h"
#include<string>
#include<vector>
#include"DataModel/Vector4.h"
#include"DataModel/Vector3.h"
#include"DataModel/Vector2.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"DataModel/Node.h"

class Shape;

class ManagementModel {
	friend class Shape;
	std::vector<MaterialData> LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	Node ReadNode(aiNode* node);
	std::vector<ModelData>modelData_;
	ModelData GetModel(int id) { return modelData_[id];}
public:
	int LoadObjFile(const std::string& directoryPath, const std::string& filename);
};