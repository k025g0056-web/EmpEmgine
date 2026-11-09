#pragma once
#include"Vector2.h"
#include"Vertex42d.h"
struct Matrix3x3{
	float m[3][3];
};

Matrix3x3 OrthographicMatrix();
Matrix3x3 ViewportMatrix();
Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2);
Matrix3x3 MakeTranslateMatrix(Vector2 translate);
Matrix3x3 inverse3(Matrix3x3 matrix);
Vector2 Transform(Vector2 vector, Matrix3x3 matrix);