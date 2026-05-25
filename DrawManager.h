#pragma once
#include"Triangle.h"
#include"DoubleTriangle.h"
#include<vector>

class DrawManager {
	std::vector<Triangle>triangles_;
	std::vector<DoubleTriangle>doubleTriangle_;
public:
	int AddTriangle(ID3D12Device* device);
	int AddDoubleTriangle(ID3D12Device* device);
	Triangle& GetTriangle(int index) { return triangles_[index]; }
	DoubleTriangle& GetDoubleTriangle(int index) { return doubleTriangle_[index]; }
	void Clear();
	void Release();
};