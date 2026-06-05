#pragma once
#include"Triangle.h"
#include"DoubleTriangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include<vector>

class DrawManager {
	std::vector<Triangle>triangles_;
	std::vector<DoubleTriangle>doubleTriangle_;
	std::vector<Sprite>sprite_;
	std::vector<Sphere>sphere_;
public:
	int AddTriangle(ID3D12Device* device);
	int AddDoubleTriangle(ID3D12Device* device);
	int AddSprite(ID3D12Device* device);
	int AddSphere(ID3D12Device* device);
	Triangle& GetTriangle(int index) { return triangles_[index]; }
	DoubleTriangle& GetDoubleTriangle(int index) { return doubleTriangle_[index]; }
	Sprite& GetSprite(int index) { return sprite_[index]; }
	Sphere& GetSphere(int index) { return sphere_[index]; }
	void Clear();
	void Release();
};