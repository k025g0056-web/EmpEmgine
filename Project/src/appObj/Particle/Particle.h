#pragma once
#include"DataModel/ParticleSet.h"
#include"DataModel/Emitter.h"
#include"DataModel/ModelData.h"
#include<vector>
#include<d3d12.h>

class Particle{
	std::vector<ParticleSet> particle_;
	Emitter emit_;
	int count_ = 0;

	void GUI();
	void PositionUpdate();
	void GeneParticle(int count);
	void Erase();
	void AllDelete();
	bool infenity = false;

public:
	void Initialize(int count);

	void Update();

	void Draw(const ModelData& model, D3D12_GPU_DESCRIPTOR_HANDLE handle);
};

