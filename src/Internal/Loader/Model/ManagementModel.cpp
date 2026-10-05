#include"ManagementModel.h"
#include<fstream>
#include<sstream>
#include<cassert>
#include<algorithm>
#include<Windows.h>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

ModelData ManagementModel::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;
	Assimp::Importer importer;
	std::string objPath = "./resources/3dObject/" + directoryPath + "/" + filename;
	
	// ★デバッグ用
	OutputDebugStringA("=== LoadObjFile ===\n");
	OutputDebugStringA(("試そうとしてるパス: " + objPath + "\n").c_str());

	const aiScene* scene = importer.ReadFile(objPath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());//メッシュがないものは対応しない。

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes;++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces;++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);
			for (uint32_t element = 0; element < face.mNumIndices;++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texCoord = { texcoord.x,texcoord.y };

				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.meshes[meshIndex].vertices.push_back(vertex);
			}
		}

	}

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials;++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE)!=0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.materials[materialIndex].textureFile = "./resources/3dObject/" + directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	return modelData;
}

std::vector<MaterialData> ManagementModel::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	std::vector<MaterialData> materials;//構築するマテリアルデータの配列
	MaterialData* currentMaterial = nullptr;
	std::string line;//ファイルから呼んだ1行を格納するもの
	std::ifstream file("./resources/3dObject/" + directoryPath + "/" + filename); // ★修正①：3dObjectのパスが抜けていたので追加
	assert(file.is_open());//とりあえず開けなかったら止める

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;//行の先頭の識別子を取得

		if (identifier == "newmtl") {
			MaterialData materialData;
			s >> materialData.name;
			materials.push_back(materialData);
			currentMaterial = &materials.back();
		}
		else if (identifier == "map_Kd") {
			if (currentMaterial == nullptr) {
				continue; // newmtlより前にmap_Kdが来ることは無い想定だが念のため
			}
			std::string texturefilename;
			s >> texturefilename;

			currentMaterial->textureFile = texturefilename; // ★修正②：directoryPathを付けず、resources/Image/直下のファイル名だけにする
			currentMaterial->hasTexture = true;
		}
	}

	return materials;
}