#include "CollisionUtils.h"
#include <math.h>
#include "RigidBody.h"

float CollisionUtils::Epsilon = 0.00001f;

// accounts for floating point errors when comparing two floats
bool CollisionUtils::equal(float a, float b)
{
	return (abs(a - b) <= CollisionUtils::Epsilon);
}

bool CollisionUtils::checkCollisionAABB(phy::RigidBody* a, phy::RigidBody* b)
{
	AABBPoints bounds1 = a->getBounds();
	AABBPoints bounds2 = b->getBounds();

	return (bounds1.intersects(bounds2));
}

// collision detection function
bool CollisionUtils::checkCollision(phy::RigidBody* a, phy::RigidBody* b)
{
	// do AABB check
	if (a->getBounds().intersects(b->getBounds()))
	{
		a->setVelocity(glm::vec2(0));
		a->setAngularVelocity(0);
		b->setVelocity(glm::vec2(0));
		b->setAngularVelocity(0);
	}

	ShapeTypes type1 = a->getShapeID();
	ShapeTypes type2 = b->getShapeID();

	return true;
}
