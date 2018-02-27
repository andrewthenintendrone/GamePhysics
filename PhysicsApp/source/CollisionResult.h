#pragma once
#include <glm\common.hpp>

// contains info about the result of a collision
struct CollisionResult
{
	bool collided; // did a collision actually occur?
	glm::vec2 axis; // collision axis
};