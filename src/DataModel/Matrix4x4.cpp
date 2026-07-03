#include"Matrix4x4.h"
#include<assert.h>
#include<cmath>

Matrix4x4 Affine(const Transform3d& transform) {
	return Affine(transform.scale, transform.rotate, transform.translate);
}

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 Ans;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Ans.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}

	return Ans;
}

//減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 Ans;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Ans.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}

	return Ans;
}

//掛け算
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 Ans;
	Ans = {
		m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0],/**/m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1],/**/m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2],/**/m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3],
		m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0],/**/m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1],/**/m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2],/**/m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3],
		m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0],/**/m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1],/**/m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2],/**/m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3],
		m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0],/**/m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1],/**/m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2],/**/m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3],
	};
	return Ans;
}

//逆行列
//--------------------------------------------------------//
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 Ans{};
	Matrix4x4 adj{};
	float det = InverseDeterminant(m);
	assert(det != 0);
	adj = InverseElement(m);
	Ans = Scalar(Transpose(adj), 1.0f / det);
	return Ans;
}

float InverseDeterminant(Matrix4x4 m) {
	return
		(m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3])
		- (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2])//1

		+ (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1])
		- (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3])//2

		+ (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2])
		- (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1])//3

		- (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3])
		+ (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2])//4

		- (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1])
		+ (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3])//5

		- (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2])
		+ (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1])//6

		+ (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3])
		- (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2])//7

		+ (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1])
		- (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3])//8

		+ (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2])
		- (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1])//9

		- (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0])
		+ (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0])//A

		- (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0])
		+ (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0])//B

		- (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0])
		+ (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]);//C
}

Matrix4x4 InverseElement(Matrix4x4 m) {
	Matrix4x4 Ans{};

	Ans.m[0][0] = (m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]));
	Ans.m[0][1] = -(m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]));
	Ans.m[0][2] = (m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));
	Ans.m[0][3] = -(m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

	Ans.m[1][0] = -(m.m[0][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[0][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]));
	Ans.m[1][1] = (m.m[0][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]));
	Ans.m[1][2] = -(m.m[0][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));
	Ans.m[1][3] = (m.m[0][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[0][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

	Ans.m[2][0] = (m.m[0][1] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][2] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) + m.m[0][3] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]));
	Ans.m[2][1] = -(m.m[0][0] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][2] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]));
	Ans.m[2][2] = (m.m[0][0] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0]));
	Ans.m[2][3] = -(m.m[0][0] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]) + m.m[0][2] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0]));

	Ans.m[3][0] = -(m.m[0][1] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][2] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) + m.m[0][3] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]));
	Ans.m[3][1] = (m.m[0][0] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][2] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][3] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]));
	Ans.m[3][2] = -(m.m[0][0] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][3] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]));
	Ans.m[3][3] = (m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) + m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]));

	return Ans;
}

//--------------------------------------------------------//

//転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 Ans;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Ans.m[i][j] = m.m[j][i];
		}
	}

	return Ans;
}

//単位行列
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 Ans;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				Ans.m[i][j] = 1.0f;
			}
			else {
				Ans.m[i][j] = 0.0f;
			}
		}
	}

	return Ans;
}

//スカラー倍
Matrix4x4 Scalar(const Matrix4x4& m, float scala) {
	Matrix4x4 Ans;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Ans.m[i][j] = m.m[i][j] * scala;
		}
	}

	return Ans;
}

//スケール
Matrix4x4 MakeScale(const Vector3& scale) {
	Matrix4x4 Ans = {};
	Ans.m[0][0] = scale.x;
	Ans.m[1][1] = scale.y;
	Ans.m[2][2] = scale.z;
	Ans.m[3][3] = 1.0f;
	return Ans;
}

//平行移動
Matrix4x4 MakeTranslate(const Vector3& translate) {
	Matrix4x4 Ans = MakeIdentity4x4();

	Ans.m[3][0] = translate.x;
	Ans.m[3][1] = translate.y;
	Ans.m[3][2] = translate.z;

	return Ans;
}

//回転
//----------------------------------------------------//
Matrix4x4 PitchRotate(float theta) {
	Matrix4x4 Ans = MakeIdentity4x4();
	Ans.m[0][0] = 1.0f;
	Ans.m[1][1] = std::cos(theta);
	Ans.m[1][2] = std::sin(theta);
	Ans.m[2][1] = -std::sin(theta);
	Ans.m[2][2] = std::cos(theta);
	Ans.m[3][3] = 1.0f;

	return Ans;
}

Matrix4x4 YawRotate(float theta) {
	Matrix4x4 Ans = MakeIdentity4x4();
	Ans.m[0][0] = std::cos(theta);
	Ans.m[0][2] = -std::sin(theta);
	Ans.m[1][1] = 1.0f;
	Ans.m[2][0] = std::sin(theta);
	Ans.m[2][2] = std::cos(theta);
	Ans.m[3][3] = 1.0f;

	return Ans;
}

Matrix4x4 RollRotate(float theta) {
	Matrix4x4 Ans = MakeIdentity4x4();

	Ans.m[0][0] = std::cos(theta);
	Ans.m[0][1] = std::sin(theta);
	Ans.m[1][0] = -std::sin(theta);
	Ans.m[1][1] = std::cos(theta);
	Ans.m[2][2] = 1.0f;
	Ans.m[3][3] = 1.0f;

	return Ans;
}

Matrix4x4 Rotate(Vector3 theta) {
	return Multiply(PitchRotate(theta.x), Multiply(YawRotate(theta.y), RollRotate(theta.z)));
}

Matrix4x4 Rotate(float theta) {
	return Multiply(PitchRotate(theta), Multiply(YawRotate(theta), RollRotate(theta)));

}

//----------------------------------------------------//

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	// [0][0], [0][1], [0][2]... と、「列」のインデックスを右に進める形に修正
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	// wが0以下（カメラの後ろ）なら、計算せずに巨大な値を返して画面外へ
	if (w <= 0.0f) {
		return { -99999.0f, -99999.0f, -99999.0f };
	}

	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}


//アフィン変換
Matrix4x4 Affine(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	return Multiply(Multiply(MakeScale(scale), Rotate(rotate)), MakeTranslate(translate));
}

//ビューポート行列
Matrix4x4 MakeViewPort(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 Ans = MakeIdentity4x4();
	Ans.m[0][0] = width / 2.0f;
	Ans.m[1][1] = -height / 2.0f;
	Ans.m[2][2] = maxDepth - minDepth;
	Ans.m[3][0] = left + (width / 2.0f);
	Ans.m[3][1] = top + (height / 2.0f);
	Ans.m[3][2] = minDepth;

	return Ans;
}

//正射影行列
Matrix4x4 MakeOrthographic(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 matrix = MakeIdentity4x4();
	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[2][2] = 1.0f / (farClip - nearClip);
	matrix.m[3][0] = (left + right) / (left - right);
	matrix.m[3][1] = (top + bottom) / (bottom - top);
	matrix.m[3][2] = (nearClip) / (nearClip - farClip);
	return matrix;

}

//透視投影行列
Matrix4x4 MakePerspectiveFov(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 matrix = {};
	matrix.m[0][0] = (1.0f / aspectRatio) * (1.0f / std::tan(fovY / 2.0f));
	matrix.m[1][1] = (1.0f / std::tan(fovY / 2.0f));
	matrix.m[2][2] = farClip / (farClip - nearClip);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	matrix.m[3][3] = 0.0f;
	return matrix;
}

Matrix4x4 Affine3D22D(const Transform3d& transform) {
	Matrix4x4 matrix = MakeScale(transform.scale);
	matrix = Multiply(matrix, RollRotate(transform.rotate.z));
	matrix = Multiply(matrix, MakeTranslate(transform.translate));
	return matrix;
}