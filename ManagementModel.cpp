#include"ManagementModel.h"
#include<fstream>
#include<sstream>
#include<cassert>

ModelData ManagementModel::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;
	std::vector<Vector4> positions;//位置
	std::vector<Vector3> normals;//法線
	std::vector<Vector2> texcoords;//テクスチャ座標
	std::string line;//ファイルから読み込んだ1行を格納する変数

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());//とりあえず開けなかったら止める

	while (std::getline(file,line)){
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;//行の先頭の識別子を取得

		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier=="vt"){
			Vector2 texCoord;
			s >> texCoord.x >> texCoord.y;
			texcoords.push_back(texCoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分割してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string Index;
					std::getline(v, Index, '/');// 「/」区切りでインデックスを呼んでいく
					elementIndices[element] = std::stoi(Index);
				}

				//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);
			}
		}
	}



	return modelData;
}