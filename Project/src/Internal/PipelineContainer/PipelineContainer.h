#pragma once
#include"DataModel/PipelineSet.h"
#include<d3d12.h>

class PipelineContainer{


	Pipeline object;
	Pipeline particle;

	void MakeObjectShaderPipeline(ID3D12Device* device);

public:
	void Initialize(ID3D12Device* device);


};

