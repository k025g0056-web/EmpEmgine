#pragma once

struct Vector4{
	float x;
	float y;
	float z;
	float w;

	Vector4 operator*(const float& other) const {
		return {
			x * other,
			y * other,
			z * other,
			w * other
		};
	}
	
	Vector4 operator/(const float& other) const {
		return {
			x / other,
			y / other,
			z / other,
			w / other
		};
	}
};

inline Vector4 operator*(float scalar, const Vector4& v) {
	return {
		v.x * scalar,
		v.y * scalar,
		v.z * scalar,
		v.w * scalar
	};
}
