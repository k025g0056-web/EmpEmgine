#include"Matrix3x3.h"
#include<cassert>

Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2) {
	Matrix3x3 matrixAnswer{};
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			matrixAnswer.m[row][column] =
				matrix1.m[row][0] * matrix2.m[0][column] +
				matrix1.m[row][1] * matrix2.m[1][column] +
				matrix1.m[row][2] * matrix2.m[2][column];
		}
	}
	return matrixAnswer;
}

Matrix3x3 MakeTranslateMatrix(Vector2 translate) {
	Matrix3x3 Answer{};
	Answer.m[0][0] = 1.0f;
	Answer.m[1][0] = 0.0f;
	Answer.m[2][0] = translate.x;
	Answer.m[0][1] = 0.0f;
	Answer.m[1][1] = 1.0f;
	Answer.m[2][1] = translate.y;
	Answer.m[0][2] = 0.0f;
	Answer.m[1][2] = 0.0f;
	Answer.m[2][2] = 1.0f;
	return Answer;
}

Matrix3x3 inverse3(Matrix3x3 matrix) {
	float denominator =
		matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +

		matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +

		matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -

		matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -

		matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -

		matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1];
	assert(denominator != 0);
	Matrix3x3 Answer{};
	Answer.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]) / denominator;
	Answer.m[0][1] = -(matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]) / denominator;
	Answer.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]) / denominator;
	Answer.m[1][0] = -(matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]) / denominator;
	Answer.m[1][1] = (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]) / denominator;
	Answer.m[1][2] = -(matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]) / denominator;
	Answer.m[2][0] = (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]) / denominator;
	Answer.m[2][1] = -(matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]) / denominator;
	Answer.m[2][2] = (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]) / denominator;
	return Answer;
}

Matrix3x3 OrthographicMatrix(float left,float top,float right,float bottom) {
	Matrix3x3 Answer{};
	Answer.m[0][0] = 2.0f / (right - left); Answer.m[0][1] = 0.0f; Answer.m[0][2] = 0.0f;
	Answer.m[1][0] = 0.0f; Answer.m[1][1] = 2.0f / (top - bottom); Answer.m[1][2] = 0.0f;
	Answer.m[2][0] = (left + right) / (left - right); Answer.m[2][1] = (top + bottom) / (bottom - top); Answer.m[2][2] = 1.0f;
	return Answer;
}

Matrix3x3 ViewportMatrix(float left, float top,float width,float height) {
	Matrix3x3 Answer{};
	Answer.m[0][0] = width/ 2.0f; Answer.m[0][1] = 0.0f; Answer.m[0][2] = 0.0f;
	Answer.m[1][0] = 0.0f; Answer.m[1][1] = -(height/ 2.0f); Answer.m[1][2] = 0.0f;
	Answer.m[2][0] = left+(width/ 2.0f); Answer.m[2][1] = top+(height / 2.0f); Answer.m[2][2] = 1.0f;
	return Answer;
}

Vector2 Transform(Vector2 vector, Matrix3x3 matrix) {
	Vector2 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0);
	result.x /= w;
	result.y /= w;
	return result;
}

