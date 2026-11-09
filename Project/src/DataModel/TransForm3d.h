#pragma once
#include"Vector3.h"

struct Transform3d{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;

	bool operator==(const Transform3d& other) const{
		return (scale == other.scale) && (rotate == other.rotate) && (translate == other.translate);
	}
};
