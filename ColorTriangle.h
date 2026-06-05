#pragma once
#include"TransForm3d.h"
#include"Vector4.h"

class ColorTriangle{
public:
	void initialize();
	void GUI();
	void Draw();

private:
	Vector3 vertice_[3];
	Vector4 color;


};

