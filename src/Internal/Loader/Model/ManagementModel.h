#pragma once
#include"DataModel/ModelData.h"
#include<string>
#include<vector>
#include"DataModel/Vector4.h"
#include"DataModel/Vector3.h"
#include"DataModel/Vector2.h"

class ManagementModel {
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
public:
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);



};