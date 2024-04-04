#pragma once

#ifndef MATHHELPER_INCLUDE
#define MATHHELPER_INCLUDE

class MathHelper
{
public:
	static float lerp(float a, float b, float f);
	static float lerpClamp(float a, float b, float f);
};

#endif