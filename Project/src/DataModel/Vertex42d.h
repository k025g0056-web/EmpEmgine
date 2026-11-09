#pragma once
#include"Vector2.h"

struct Vertex42d{
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftBottom;
	Vector2 rightBottom;
};

Vertex42d DefinitionVertex(float width, float height);