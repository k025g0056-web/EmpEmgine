#pragma once
#include"ModelData.h"
#include<string>
#include<vector>
#include"Vector4.h"
#include"Vector3.h"
#include"Vector2.h"

class ManagementModel {
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
public:
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);



};