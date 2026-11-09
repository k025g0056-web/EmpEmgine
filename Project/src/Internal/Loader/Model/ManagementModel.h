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

class ManagementModel {
	std::vector<MaterialData> LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	Node ReadNode(aiNode* node);
public:
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);



};