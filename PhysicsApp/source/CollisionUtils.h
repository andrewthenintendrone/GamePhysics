#pragma once
#include "Projection.h"
#include "RigidBody.h"
#include <glm\ext.hpp>
#include "AABBPoints.h"
#include "Polygon.h"

class CollisionUtils
{
public:

	static bool equal(float a, float b);

	static float Epsilon;

	static bool checkCollisionAABB(phy::RigidBody* a, phy::RigidBody* b);

	static float calculateJ(phy::RigidBody* a, phy::RigidBody* b, glm::vec2 collisionNormal, glm::vec2 collisionPoint);

	static bool checkCollision(phy::RigidBody* a, phy::RigidBody* b);

	static bool checkCollisionSAT(phy::RigidBody* a, phy::RigidBody* b, glm::vec2& collisionNormal, glm::vec2& collisionPoint);

	static void resolveCollision(phy::RigidBody* a, phy::RigidBody* b, glm::vec2 collisionPoint, glm::vec2* collisionNormal = nullptr);
};