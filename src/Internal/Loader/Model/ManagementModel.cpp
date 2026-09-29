#include"ManagementModel.h"
#include<fstream>
#include<sstream>
#include<cassert>
#include<algorithm>
#include<Windows.h>


ModelData ManagementModel::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;
	std::string objPath = "./resources/3dObject/" + directoryPath + "/" + filename;

	// ★デバッグ用
	OutputDebugStringA("=== LoadObjFile ===\n");
	OutputDebugStringA(("試そうとしてるパス: " + objPath + "\n").c_str());

	std::ifstream file(objPath);
	assert(file.is_open());

	MeshData* currentMesh = nullptr;
	std::string currentObjectName = ""; // 追加：今読んでいるオブジェクト名

	// マテリアル名とオブジェクト名の両方が一致するメッシュを探す関数
	auto findOrCreateMesh = [&](const std::string& objectName, const std::string& materialName) -> MeshData* {
		auto it = std::find_if(modelData.meshes.begin(), modelData.meshes.end(),
			[&](const MeshData& mesh) {
				return mesh.objectName == objectName && mesh.materialName == materialName;
			});

		if (it != modelData.meshes.end()) {
			return &(*it);
		}

		MeshData newMesh;
		newMesh.objectName = objectName;
		newMesh.materialName = materialName;
		modelData.meshes.push_back(newMesh);
		return &modelData.meshes.back();
		};

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texCoord;
			s >> texCoord.x >> texCoord.y;
			texCoord.y = 1.0f - texCoord.y;
			texCoord.x = 1.0f - texCoord.x;
			texcoords.push_back(texCoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "o" || identifier == "g") {
			// 新しいオブジェクト(またはグループ)の始まり
			s >> currentObjectName;
			currentMesh = nullptr; // マテリアル指定はオブジェクトごとにやり直しになるのでリセット
		}
		else if (identifier == "usemtl") {
			std::string materialName;
			s >> materialName;
			currentMesh = findOrCreateMesh(currentObjectName, materialName);
		}
		else if (identifier == "f") {
			if (currentMesh == nullptr) {
				currentMesh = findOrCreateMesh(currentObjectName, "");
			}

			// 面の頂点を全部読み込む(3つとは限らない)
			std::vector<VertexData> faceVertices;
			std::string vertexDefinition;
			while (s >> vertexDefinition) {
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3] = { 0, 0, 0 };
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					if (!index.empty()) {
						elementIndices[element] = std::stoi(index);
					}
				}

				Vector4 position = positions[elementIndices[0] - 1];

				Vector2 texcoord = { 0.0f, 0.0f };
				if (elementIndices[1] != 0) {
					texcoord = texcoords[elementIndices[1] - 1];
				}

				Vector3 normal = { 0.0f, 1.0f, 0.0f };
				if (elementIndices[2] != 0) {
					normal = normals[elementIndices[2] - 1];
				}

				position.x *= -1.0f;
				normal.x *= -1.0f;

				faceVertices.push_back({ position, texcoord, normal });
			}

			// 扇形分割：3頂点なら三角形1つ、4頂点なら三角形2つ...
			for (size_t i = 1; i + 1 < faceVertices.size(); ++i) {
				currentMesh->vertices.push_back(faceVertices[i + 1]);
				currentMesh->vertices.push_back(faceVertices[i]);
				currentMesh->vertices.push_back(faceVertices[0]);
			}
		}
		else if (identifier == "mtllib") {
			std::string materialFilename;
			s >> materialFilename;
			modelData.materials = LoadMaterialTemplateFile(directoryPath, materialFilename);
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