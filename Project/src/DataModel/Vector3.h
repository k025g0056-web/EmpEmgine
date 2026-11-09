#pragma once
#include"Vector4.h"

struct Vector3 {
	float x;
	float y;
	float z;

	Vector3 operator+(const Vector3& other) const {
		return {
			x + other.x,
			y + other.y,
			z + other.z,
		};
	}

	Vector3 operator-(const Vector3& other) const {
		return {
			x - other.x,
			y - other.y,
			z - other.z,
		};
	}

	Vector3 operator*(const Vector3& other) const {
		return {
			x * other.x,
			y * other.y,
			z * other.z,
		};
	}

	Vector3 operator/(const Vector3& other) const {
		return {
			x / other.x,
			y / other.y,
			z / other.z,
		};
	}

	Vector3& operator=(const Vector3& other){
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vector3 operator*(const float& other) const {
		return {
			x * other,
			y * other,
			z * other,
		};
	}

	Vector3 operator/(const float& other) const {
		return {
			x / other,
			y / other,
			z / other,
		};
	}

	bool operator==(const Vector3& other) const {
		return{
			(x == other.x) && (y == other.y) && (z == other.z)
		};
	}

	bool operator&&(const Vector3& other) const {
		return {(x&& other.x)&&(y&&other.y)&&(z&&other.z)};
	}

	Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

};

float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 Normalize(const Vector3& v);
Vector3 Cross(const Vector3& v1, const Vector3& v2);
inline Vector3 ReturnAllOne() { return { 1.0f,1.0f,1.0f }; }
inline Vector3 operator*(float scalar, const Vector3& v) {
	return {
		v.x * scalar,
		v.y * scalar,
		v.z * scalar,
	};
}

inline Vector3& operator*=(Vector3& v, float scalar) {
	v.x *= scalar;
	v.y *= scalar;
	v.z *= scalar;
	return v;
}


inline Vector3 operator/(float scalar, const Vector3& v) {
	return {
		v.x / scalar,
		v.y / scalar,
		v.z / scalar,
	};
}
Vector3 Clamp(const Vector3& vec, float min,float max);

inline Vector3 ToVec3(const Vector4& vec) {
	return { vec.x,vec.y,vec.z };
}

inline Vector3 ToVec3Sprite(const Vector4& vec) {
	return { vec.x,vec.y,-1.0f };
}