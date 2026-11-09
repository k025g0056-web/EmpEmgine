#include"DrawManager.h"



void DrawManager::Clear() {
	triangles_.clear();
	doubleTriangle_.clear();
	sprite_.clear();
	sphere_.clear();
	model_.clear();
	particle3D_.clear();
	
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

	for (auto& mod:model_) {
		//mod.ReleaseModel();
		mod.Release();
	}

	for (auto& par3:particle3D_) {
		//par3.ReleaseModel();
		par3.Release();
	}

	Clear();
}