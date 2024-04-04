#include "MathHelper.h"
#include <iostream>

float MathHelper::lerp(float a, float b, float f)
{
	return a * (1.0 - f) + (b * f);
}

float MathHelper::lerpClamp(float a, float b, float f)
{
	if (f > 1.0f)
		f = 1.0f;

	return lerp(a, b, f);
}