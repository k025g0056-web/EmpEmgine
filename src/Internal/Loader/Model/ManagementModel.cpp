#include"ManagementModel.h"
#include<fstream>
#include<sstream>
#include<cassert>
#include<algorithm>

ModelData ManagementModel::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;
	std::vector<Vector4> positions;//位置
	std::vector<Vector3> normals;//法線
	std::vector<Vector2> texcoords;//テクスチャ座標
	std::string line;//ファイルから読み込んだ1行を格納する変数
	VertexData triangle[3]{};

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());//とりあえず開けなかったら止める

	// 現在書き込み中のメッシュ(usemtlが来るたびに切り替える)
	MeshData* currentMesh = nullptr;

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;//行の先頭の識別子を取得

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
		else if (identifier == "usemtl") {
			std::string materialName;
			s >> materialName;

			// 同じマテリアル名のメッシュが既にあればそこに追加し続ける
			auto it = std::find_if(modelData.meshes.begin(), modelData.meshes.end(),
				[&](const MeshData& mesh) { return mesh.materialName == materialName; });

			if (it != modelData.meshes.end()) {
				currentMesh = &(*it);
			}
			else {
				MeshData newMesh;
				newMesh.materialName = materialName;
				modelData.meshes.push_back(newMesh);
				currentMesh = &modelData.meshes.back();
			}
		}
		else if (identifier == "f") {
			// usemtlが一度も出てきていないobjファイル用のデフォルトメッシュ
			if (currentMesh == nullptr) {
				MeshData newMesh;
				newMesh.materialName = ""; // マテリアル未指定
				modelData.meshes.push_back(newMesh);
				currentMesh = &modelData.meshes.back();
			}

			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分割してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3] = { 0, 0, 0 }; // 0 = 未指定
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');// 「/」区切りでインデックスを呼んでいく
					if (!index.empty()) {
						elementIndices[element] = std::stoi(index);
					}
					// 空文字列なら0のまま(該当要素なし = UVや法線が無い面)
				}

				//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];

				// UVが無い場合はデフォルト値にフォールバック
				Vector2 texcoord = { 0.0f, 0.0f };
				if (elementIndices[1] != 0) {
					texcoord = texcoords[elementIndices[1] - 1];
				}

				// 法線が無い場合もデフォルト値にフォールバック
				Vector3 normal = { 0.0f, 1.0f, 0.0f };
				if (elementIndices[2] != 0) {
					normal = normals[elementIndices[2] - 1];
				}

				position.x *= -1.0f;
				normal.x *= -1.0f;
				triangle[faceVertex] = { position,texcoord,normal };
			}

			//By registering the vertices in reverse order, the rotation order is reversed.
			currentMesh->vertices.push_back(triangle[2]);
			currentMesh->vertices.push_back(triangle[1]);
			currentMesh->vertices.push_back(triangle[0]);
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
	std::ifstream file(directoryPath + "/" + filename);
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

			currentMaterial->textureFile = directoryPath + "/" + texturefilename;
			currentMaterial->hasTexture = true;
		}
	}

	return materials;
}