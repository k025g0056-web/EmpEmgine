#pragma once
#include"TransForm3d.h"
#include"Vector3.h"
#include"Vector4.h"

struct ParticleSet{
	Transform3d transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};