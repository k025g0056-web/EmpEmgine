#include "Particle.h"
#include "core/wrapper/EmpEngine.h"
#include "externals/imgui/imgui.h"

void Particle::Initialize(int count) {
	count_ = count;
    GeneParticle(count_);
}

void Particle::Update() {
    GUI();

    GeneParticle(count_);
    PositionUpdate();
}

void Particle::Draw(ModelData model, D3D12_GPU_DESCRIPTOR_HANDLE handle) {
	std::vector<Transform3d>transform;
    std::vector<Vector4> colo;
    int numInstance = 0;

	for (int i = 0; i < particle_.size();i++) {
        if (particle_[i].lifeTime<=particle_[i].currentTime) {
            continue;
        }
        particle_[i].currentTime += EmpEngine::DeltaTime();
		transform.push_back(particle_[i].transform);
        colo.push_back(particle_[i].color);
        ++numInstance;
	}

	EmpEngine::Draw().DrawModel<Particle3D>(model, transform, handle,colo,numInstance);
}

void Particle::GeneParticle(int count) {

    int countI = count - static_cast<int>(particle_.size());
    
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
    std::uniform_real_distribution<float> distTime(1.0f, 3.0f);

    for (int i = 0; i < countI; i++) {
        Transform3d transform{};
        transform.scale = { 1.0f,1.0f,1.0f };
        transform.rotate = { 0.0f,0.0f,0.0f };
        transform.translate = { distribution(EmpEngine::GetRandom()),distribution(EmpEngine::GetRandom()) ,distribution(EmpEngine::GetRandom()) };
        Vector3 velo = { distribution(EmpEngine::GetRandom()),distribution(EmpEngine::GetRandom()) ,distribution(EmpEngine::GetRandom()) };
        Vector4 col= { distribution(EmpEngine::GetRandom()),distribution(EmpEngine::GetRandom()) ,distribution(EmpEngine::GetRandom()),1.0f };
        particle_.push_back({ transform ,velo,col,distTime(EmpEngine::GetRandom()),0.0f});
    }
}

void Particle::PositionUpdate() {
    for (int i = 0; i < particle_.size(); i++) {
        particle_[i].transform.translate += particle_[i].velocity * EmpEngine::DeltaTime();
    }
}

void Particle::GUI() {
#ifdef USE_IMGUI
    ImGui::Begin("Particle");
    ImGui::SliderInt("Count", &count_, 1, 100000);
    ImGui::End();
#endif // USE_IMGUI
}