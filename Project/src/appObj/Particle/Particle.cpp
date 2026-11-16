#include "Particle.h"
#include "core/wrapper/EmpEngine.h"
#include "externals/imgui/imgui.h"

void Particle::Initialize(int count) {
	count_ = count;
    GeneParticle(count_);
}

void Particle::Update() {
    GUI();

    if (infenity) {
        GeneParticle(count_);
    }

    Erase();
    PositionUpdate();
}

void Particle::Draw(const ModelData& model, D3D12_GPU_DESCRIPTOR_HANDLE handle) {
	std::vector<Transform3d>transform;
    std::vector<Vector4> colo;
    transform.reserve(particle_.size());
    colo.reserve(particle_.size());
   
	for (int i = 0; i < particle_.size();i++) {
		transform.push_back(particle_[i].transform);
        colo.push_back(particle_[i].color);
	}

	EmpEngine::Draw().DrawModel<Particle3D>(model, transform, handle,colo, static_cast<int>(transform.size()));
}

void Particle::GeneParticle(int count) {

    int countI = count - static_cast<int>(particle_.size());
    
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
    std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
    std::uniform_real_distribution<float> distColor(0.0f, 1.0f);

    for (int i = 0; i < countI; i++) {
        Transform3d transform{};
        transform.scale = { 1.0f,1.0f,1.0f };
        transform.rotate = { 0.0f,0.0f,0.0f };
        transform.translate = { distribution(EmpEngine::GetRandom()),distribution(EmpEngine::GetRandom()) ,distribution(EmpEngine::GetRandom()) };
        transform.translate = emit_.transform.translate;
        Vector3 velo = { distribution(EmpEngine::GetRandom()),distribution(EmpEngine::GetRandom()) ,distribution(EmpEngine::GetRandom()) };
        Vector4 col= { distColor(EmpEngine::GetRandom()),distColor(EmpEngine::GetRandom()) ,distColor(EmpEngine::GetRandom()),1.0f };
        particle_.push_back({ transform ,velo,col,distTime(EmpEngine::GetRandom()),0.0f});
    }
}

void Particle::PositionUpdate() {
    for (int i = 0; i < particle_.size(); i++) {
        particle_[i].currentTime += EmpEngine::DeltaTime();
        particle_[i].color.w = 1.0f - particle_[i].currentTime / particle_[i].lifeTime;
        particle_[i].transform.translate += particle_[i].velocity * EmpEngine::DeltaTime();
        particle_[i].transform.rotate = SceneSystem::GetCamera()->GetCameraRotate();
    }
}

void Particle::GUI() {
#ifdef USE_IMGUI
    ImGui::Begin("Particle");
    if (ImGui::SliderInt("Count", &count_, 1, 100000)) {
        AllDelete();
        GeneParticle(count_);
    }

    if (infenity) {
        if (ImGui::Button("Infenity off")) {
            infenity = false;
        }

    }else {
        if (ImGui::Button("Infenity on")) {
            infenity = true;
        }

    }

    if (ImGui::Button("All Delete")) {
        AllDelete();
    }

    if (ImGui::Button("Respawn")) {
        GeneParticle(count_);
    }

    ImGui::SliderFloat3("Emiter Position", &emit_.transform.translate.x, -3.0f, 3.0f);

    ImGui::End();
#endif // USE_IMGUI
}

void Particle::Erase() {
    particle_.erase(
        std::remove_if(particle_.begin(), particle_.end(),
            [](const ParticleSet& p) {return p.lifeTime <= p.currentTime; })
        , particle_.end());
}

void Particle::AllDelete() {
    particle_.clear();
}