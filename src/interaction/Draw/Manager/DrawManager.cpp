#include"DrawManager.h"

int DrawManager::AddTriangle(ID3D12Device* device) {
	triangles_.emplace_back();
	triangles_.back().Initialize(device);
	return static_cast<int>(triangles_.size() - 1);
}

int DrawManager::AddDoubleTriangle(ID3D12Device* device) {
	doubleTriangle_.emplace_back();
	doubleTriangle_.back().Initialize(device);
	return static_cast<int>(doubleTriangle_.size() - 1);
}

int DrawManager::AddSprite(ID3D12Device* device) {
	sprite_.emplace_back();
	sprite_.back().Initialize(device);
	return static_cast<int>(sprite_.size() - 1);
}

int DrawManager::AddSphere(ID3D12Device* device) {
	sphere_.emplace_back();
	sphere_.back().Initialize(device);
	return static_cast<int>(sphere_.size() - 1);
}

int DrawManager::AddModel(ID3D12Device* device,const ModelData& modelData) {
	model_.emplace_back();
	model_.back().Initialize(device,modelData);
	return static_cast<int>(model_.size() - 1);
}

void DrawManager::Clear() {
	triangles_.clear();
	doubleTriangle_.clear();
	sprite_.clear();
	sphere_.clear();
}

void DrawManager::Release() {
	for (auto& tri:triangles_) {
		tri.Release();
	}

	for (auto& dou:doubleTriangle_) {
		dou.Release();
	}

	for (auto& spr:sprite_) {
		spr.Release();
	}

	for (auto& sph : sphere_){
		sph.Release();
	}

	triangles_.clear();
	doubleTriangle_.clear();
	sprite_.clear();
	sphere_.clear();
}