#pragma once
#include "Projection.h"
#include "RigidBody.h"
#include <glm\ext.hpp>
#include "AABBPoints.h"

class CollisionUtils
{
public:

	static bool equal(float a, float b);

	static float Epsilon;

	static bool checkCollisionAABB(phy::RigidBody* a, phy::RigidBody* b);

	static bool checkCollision(phy::RigidBody* a, phy::RigidBody* b);
};