#include"DrawManager.h"



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