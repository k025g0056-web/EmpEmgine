#include"Vertex42d.h"
#include <cmath>

Vertex42d DefinitionVertex(float width, float height) {
	Vertex42d top;
	top.leftTop = { -width / 2.0f,height / 2.0f };
	top.rightTop = { width / 2.0f,height / 2.0f };
	top.leftBottom = { -width / 2.0f,-height / 2.0f };
	top.rightBottom = { width / 2.0f,-height / 2.0f };
	return top;
}