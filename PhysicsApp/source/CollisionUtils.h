#pragma once
#include <glm\ext.hpp>

// represents a projection of a vec2 along a normal
struct Projection
{
	// default constructor
	Projection() {};

	// constructor with point and dot
	Projection(glm::vec2 apoint, float adotAlongNormal)
	{
		point = apoint;
		dotAlongNormal = adotAlongNormal;
	}

	// constructor with normal
	Projection(glm::vec2 apoint, glm::vec2 anormal)
	{
		point = apoint;
		dotAlongNormal = glm::dot(point, anormal);
	}

	glm::vec2 point = glm::vec2(0);
	float dotAlongNormal = 0;
};

class CollisionUtils
{
public:

	static bool equal(float a, float b);

	static bool sortByDot(Projection& a, Projection& b);

	static float getProjectionOverlap(float minA, float maxA, float minB, float maxB);

	static float Epsilon;
};