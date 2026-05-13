#pragma once
#include"Matrix4x4.h"
#include"Vector3.h"


Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Inverse(const Matrix4x4& m);
float InverseDeterminant(Matrix4x4 m);
Matrix4x4 InverseElement(Matrix4x4 m);
Matrix4x4 Transpose(const Matrix4x4& m);
Matrix4x4 Scalar(const Matrix4x4& m, float scala);
Matrix4x4 MakeScale(const Vector3& scale);
Matrix4x4 MakeTranslate(const Vector3& translate);
Matrix4x4 PitchRotate(float theta);
Matrix4x4 YawRotate(float theta);
Matrix4x4 RollRotate(float theta);
Matrix4x4 Rotate(Vector3 theta);
Matrix4x4 Rotate(float theta);
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
Matrix4x4 Affine(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Matrix4x4 MakeViewPort(float left, float top, float width, float height, float minDepth, float maxDepth);
Matrix4x4 MakeOrthographic(float left, float top, float right, float bottom, float nearClip, float farClip);
Matrix4x4 MakeIdentity4x4();
Matrix4x4 MakePerspectiveFov(float fovY, float aspectRatio, float nearClip, float farClip);
