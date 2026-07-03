#define NOMINMAX
#include"Vector3.h"
#include<math.h>
#include<algorithm>

//外積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return{ v1.y * v2.z - v1.z * v2.y,v1.z * v2.x - v1.x * v2.z,v1.x * v2.y - v1.y * v2.x };
}

//内積
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//長さ
float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

//正規化
Vector3 Normalize(const Vector3& v) {
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	Vector3 Answer;
	if (length == 0.0f) {
		return { 0.0f,0.0f,0.0f };
	} else {
		Answer.x = v.x / length;
		Answer.y = v.y / length;
		Answer.z = v.z / length;
		return Answer;
	}
}

Vector3 Clamp(const Vector3& vec, float min,float  max) {
	Vector3 vector;
	vector.x = std::clamp(vec.y, min, max);
	vector.y = std::clamp(vec.y, min, max);
	vector.y = std::clamp(vec.y, min, max);
	return vector;
}