#pragma once
#include"Interaction/Draw/Triangle/Triangle.h"
#include"Interaction/Draw/DoubleTriangle/DoubleTriangle.h"
#include"Interaction/Draw/Sprite/Sprite.h"
#include"Interaction/Draw/Sphere/Sphere.h"
#include<vector>
#include"Interaction/Draw/Model/Model.h"
#include"Scene/wrapper/SceneSystem.h"
#include <functional>


class DrawManager {
	std::vector<Triangle>triangles_;
	std::vector<DoubleTriangle>doubleTriangle_;
	std::vector<Sprite>sprite_;
	std::vector<Sphere>sphere_;
	std::vector<Model>model_;
	template<typename T>
	std::vector<T>& GetVector();

	ID3D12GraphicsCommandList* commandList_ = nullptr;
	ID3D12Device* device_ = nullptr;
	template<typename T>
	int& UsedCount();
	int usedTriangle_ = 0;
	int usedDoubleTriangle_ = 0;
	int usedSprite_ = 0;
	int usedSphere_ = 0;
	int usedModel_ = 0;

	std::function<void()> postDrawFunc_;
public:
	void Initialize(ID3D12Device* device) { device_ = device; }

	void SetPostDrawFunc(std::function<void()> func) {
		postDrawFunc_ = func;
	}

	template<typename T, typename... Args>
	int Add(Args&&... args) {
		auto& vec = GetVector<T>();
		int& used = UsedCount<T>();

		if (used < static_cast<int>(vec.size())) {
			return used++;
		}

		vec.emplace_back();
		vec.back().Initialize(device_, std::forward<Args>(args)...);
		return used++;
	}

	template<typename T, typename... Args>
	void Draw(Args&&... args) {
		int idx = Add<T>();
		if (postDrawFunc_) {
			postDrawFunc_();
		}
		Get<T>(idx).Draw(commandList_, *SceneSystem::GetCamera(), std::forward<Args>(args)...);
	}

	template<typename... Args>
	void DrawModel(const ModelData& modelData, Args&&... args) {
		int idx = Add<Model>(modelData);
		if (postDrawFunc_) {
			postDrawFunc_();
		}
		Get<Model>(idx).Draw(commandList_, *SceneSystem::GetCamera(), std::forward<Args>(args)...);
	}

	template<typename T>
	T& Get(int index) {
		return GetVector<T>()[index];
	}
	void Clear();
	void Release();
	void SetCommandList(ID3D12GraphicsCommandList* commandList) { commandList_ = commandList; }

	void ResetUsedCount() {
		usedTriangle_ = 0;
		usedDoubleTriangle_ = 0;
		usedSprite_ = 0;
		usedSphere_ = 0;
		usedModel_ = 0;
	}
}; 

template<> inline std::vector<Triangle>& DrawManager::GetVector<Triangle>() { return triangles_; }
template<> inline std::vector<DoubleTriangle>& DrawManager::GetVector<DoubleTriangle>() { return doubleTriangle_; }
template<> inline std::vector<Sprite>& DrawManager::GetVector<Sprite>() { return sprite_; }
template<> inline std::vector<Sphere>& DrawManager::GetVector<Sphere>() { return sphere_; }
template<> inline std::vector<Model>& DrawManager::GetVector<Model>() { return model_; }
template<> inline int& DrawManager::UsedCount<Triangle>() { return usedTriangle_; }
template<> inline int& DrawManager::UsedCount<DoubleTriangle>() { return usedDoubleTriangle_; }
template<> inline int& DrawManager::UsedCount<Sprite>() { return usedSprite_; }
template<> inline int& DrawManager::UsedCount<Sphere>() { return usedSphere_; }
template<> inline int& DrawManager::UsedCount<Model>() { return usedModel_; }