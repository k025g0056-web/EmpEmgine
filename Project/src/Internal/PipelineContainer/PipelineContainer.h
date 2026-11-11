#pragma once
#include"DataModel/PipelineSet.h"
#include<d3d12.h>
#include"Internal/ShaderCompiler/ShaderCompiler.h"

class PipelineContainer{
	ShaderCompiler shaderCompiler_;

	Pipeline object;
	Pipeline particle;

	void MakeObjectShaderPipeline(ID3D12Device* device);

public:
	void Initialize(ID3D12Device* device);

	Pipeline GetObjectPipe() { return object; }
	Pipeline GetPartoclePipe() { return particle; }

};

