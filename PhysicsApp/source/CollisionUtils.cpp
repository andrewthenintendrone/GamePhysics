#include "CollisionUtils.h"
#include <math.h>

float CollisionUtils::Epsilon = 0.00001f;

// accounts for floating point errors when comparing two floats
bool CollisionUtils::equal(float a, float b)
{
	return (abs(a - b) <= CollisionUtils::Epsilon);
}

// compare function used to sort Projections in order of dot along normal
bool CollisionUtils::sortByDot(Projection& a, Projection& b)
{
	return (a.dotAlongNormal > b.dotAlongNormal);
}

float CollisionUtils::getProjectionOverlap(float minA, float maxA, float minB, float maxB)
{
	return std::fmin(maxA, maxB) - std::fmax(minA, minB);
}
