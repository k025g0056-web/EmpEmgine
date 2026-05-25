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

void DrawManager::Clear() {
	triangles_.clear();
	doubleTriangle_.clear();
}

void DrawManager::Release() {
	for (auto& tri:triangles_) {
		tri.Release();
	}

	for (auto& dou:doubleTriangle_) {
		dou.Release();
	}

	triangles_.clear();
	doubleTriangle_.clear();
}