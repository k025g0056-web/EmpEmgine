#pragma once
#include"TransForm3d.h"
#include<cstdint>

struct  Emitter{
	Transform3d transform;
	uint32_t count;
	float frequency;
	float frequencyTime;

};
