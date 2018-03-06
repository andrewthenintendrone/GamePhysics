#include "CollisionUtils.h"
#include <math.h>

float CollisionUtils::Epsilon = 0.00001f;

// accounts for floating point errors when comparing two floats
bool CollisionUtils::equal(float a, float b)
{
	return (abs(a - b) <= CollisionUtils::Epsilon);
}
