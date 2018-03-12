#include "CollisionUtils.h"
#include <math.h>
#include "RigidBody.h"
#include "Plane.h"
#include "Sphere.h"
#include <Gizmos.h>

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

// calculate j
float CollisionUtils::calculateJ(phy::RigidBody* a, phy::RigidBody* b, glm::vec2 collisionNormal, glm::vec2 collisionPoint)
{
	float j1, j2;

	float relativeVelocityAlongNormal = glm::dot(a->getVelocity() - b->getVelocity(), collisionNormal);

	// use average elasticity of objects
	float elasticity = (a->getElasticity() + b->getElasticity()) * 0.5f;

	// top part of j equation
	j1 = -(1.0f + elasticity) * relativeVelocityAlongNormal;

	glm::vec2 pA, pB;

	pA = collisionPoint - a->getPosition();
	pB = collisionPoint - b->getPosition();

	float perpA, perpB;

	perpA = glm::dot(glm::vec2(-pA.y, pA.x), collisionNormal);
	perpB = glm::dot(glm::vec2(-pB.y, pB.x), collisionNormal);

	perpA *= perpA;
	perpB *= perpB;

	perpA /= a->getMoment();
	perpB /= b->getMoment();

	// bottom part of j equation
	j2 = glm::dot(collisionNormal, collisionNormal) * (a->invMass(), b->invMass()) + perpA + perpB;

	return j1 / j2;
}

// collision detection function
bool CollisionUtils::checkCollision(phy::RigidBody* a, phy::RigidBody* b)
{
	// do AABB check
	if (!a->getBounds().intersects(b->getBounds()))
	{
		return false;
	}

	// keep track of collision normal (always points from b to a)
	glm::vec2 collisionNormal = a->getPosition() - b->getPosition();

	glm::vec2 collisionPoint = 0.5f * (a->getPosition() + b->getPosition());

	// store types of objects
	ShapeTypes type1 = a->getShapeID();
	ShapeTypes type2 = b->getShapeID();

	// when a plane is involved the collision normal is the planes normal
	if (type1 == PLANE)
	{
		collisionNormal = -dynamic_cast<phy::Plane*>(a)->getNormal();
	}
	if (type2 == PLANE)
	{
		collisionNormal = dynamic_cast<phy::Plane*>(b)->getNormal();
	}
	
	if (a->getShapeID() == POLYGON && b->getShapeID() == POLYGON)
	{
		if(checkCollisionSAT(a, b, collisionNormal, collisionPoint))
		{
			if (glm::dot(a->getVelocity() - b->getVelocity(), collisionNormal) < 0)
			{
				resolveCollision(a, b, collisionPoint, &collisionNormal);
			}
		}
	}

	if (a->getShapeID() == SPHERE && b->getShapeID() == PLANE)
	{
		phy::Sphere* sphere = dynamic_cast<phy::Sphere*>(a);
		phy::Plane* plane = dynamic_cast<phy::Plane*>(b);

		collisionNormal = plane->getNormal();

		float sphere2Plane = glm::dot(sphere->getPosition(), collisionNormal) - plane->getDistance();

		if (sphere2Plane < sphere->getRadius())
		{
			collisionPoint = sphere->getPosition() - plane->getNormal() * sphere->getRadius();
			resolveCollision(a, b, collisionPoint, &collisionNormal);
		}
	}

	// relative velocity along collision normal must be less than 0
	// objects must be moving towards each other
	/*if (glm::dot(a->getVelocity() - b->getVelocity(), collisionNormal) < 0)
	{
		aie::Gizmos::add2DCircle(collisionPoint, 3, 8, glm::vec4(1));
		resolveCollision(a, b, collisionPoint, &collisionNormal);
	}*/

	return true;
}

bool CollisionUtils::checkCollisionSAT(phy::RigidBody* a, phy::RigidBody* b, glm::vec2& collisionNormal, glm::vec2& collisionPoint)
{
	// ensure polygons
	phy::Polygon* polygon1 = reinterpret_cast<phy::Polygon*>(a);
	phy::Polygon* polygon2 = reinterpret_cast<phy::Polygon*>(b);

	if (polygon1 != nullptr && polygon2 != nullptr)
	{
		// get points and normals
		std::vector<glm::vec2> points1 = polygon1->getLocalPointsInWorldSpace();
		std::vector<glm::vec2> points2 = polygon2->getLocalPointsInWorldSpace();

		std::vector<glm::vec2> normals1 = polygon1->getLocalNormals();
		std::vector<glm::vec2> normals2 = polygon2->getLocalNormals();

		// minimum penetration vector
		glm::vec2 minimumPenetrationVector;

		// do Projections
		Projection proj1;
		Projection proj2;

		for (std::vector<glm::vec2>::iterator iter = normals1.begin(); iter != normals1.end(); iter++)
		{
			proj1.projectPolygon(polygon1, *iter);
			proj1.projectPolygon(polygon2, *iter);
		}
		for (std::vector<glm::vec2>::iterator iter = normals2.begin(); iter != normals2.end(); iter++)
		{
			proj2.projectPolygon(polygon1, *iter);
			proj2.projectPolygon(polygon2, *iter);
		}

		if (proj1.contains(proj2) || proj1.overlaps(proj2))
		{
			return true;
		}
	}

	return false;
}

// resolve collision between rigidbodies
void CollisionUtils::resolveCollision(phy::RigidBody* a, phy::RigidBody* b, glm::vec2 collisionPoint, glm::vec2* collisionNormal)
{
	// use or calculate collision normal
	glm::vec2 normal = collisionNormal != nullptr ? *collisionNormal : a->getPosition() - b->getPosition();

	// calculate j
	float j = CollisionUtils::calculateJ(a, b, normal, collisionPoint);

	// use j to change the velocities of a and b without changing the ammount of momentum
	a->setVelocity(a->getVelocity() + (j / a->getMass()) * normal);
	b->setVelocity(b->getVelocity() - (j / b->getMass()) * normal);

	glm::vec2 pA = collisionPoint - a->getPosition();
	glm::vec2 pB = collisionPoint - b->getPosition();

	// rotational velocity
	a->setAngularVelocity(a->getAngularVelocity() - glm::dot(glm::vec2(-pA.y, -pA.x), j * *collisionNormal) / a->getMoment());
	b->setAngularVelocity(b->getAngularVelocity() + glm::dot(glm::vec2(-pB.y, -pB.x), -j * *collisionNormal) / b->getMoment());
}