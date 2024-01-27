#include "MathHelper.h"

float MathHelper::lerp(float a, float b, float f)
{
	return a * (1.0 - f) + (b * f);
}