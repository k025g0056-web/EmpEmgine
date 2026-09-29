#pragma once
#include"src/DataModel/Vector3.h"
#include"src/DataModel/ModelData.h"
#include<d3d12.h>

class Enclosure {
	Vector3 point_[4]{};
	bool isSet_[4]{};
	bool isAllSet_{};
	D3D12_GPU_DESCRIPTOR_HANDLE texture_{};
	ModelData modelData_{};

public:
	void Initialize(ModelData modelData, D3D12_GPU_DESCRIPTOR_HANDLE texture);

	void Draw();

    void Update();

    void SetPosition(Vector3 position) {
        for (int i = 0; i < 4; i++) {
            if (!isSet_[i]) {
                point_[i] = position;
                isSet_[i] = true;
                break;
            }
        }

        isAllSet_ =
            isSet_[0] &&
            isSet_[1] &&
            isSet_[2] &&
            isSet_[3];
    }


};


