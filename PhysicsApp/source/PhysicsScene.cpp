#include "PhysicsScene.h"
#include <algorithm>
#include <list>
#include "RigidBody.h"
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "Polygon.h"
#include <glm\ext.hpp>
#include "CollisionUtils.h"

// function pointer array for doing our collisions
//typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
//
//static fn collisionFunctionArray[] =
//{
//	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2Polygon,
//	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Polygon,
//	PhysicsScene::polygon2Plane, PhysicsScene::polygon2Sphere, PhysicsScene::polygon2Polygon
//};

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
	
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::update(float deltaTime)
{
	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			phy::RigidBody* pRigid = dynamic_cast<phy::RigidBody*>(pActor);
			if (pRigid != nullptr)
			{
				pActor->fixedUpdate(m_gravity, m_timeStep);
				checkCollisions();
			}
		}

		accumulatedTime -= m_timeStep;
	}
}

// draw all actors
void PhysicsScene::draw()
{
	for (auto pActor : m_actors)
	{
		pActor->draw();
	}
}

// check and resolve collisions between objects
void PhysicsScene::checkCollisions()
{
	int actorCount = (int)m_actors.size();

	// need to check for collisions against all objects except this one
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			ShapeTypes type1 = m_actors[outer]->getShapeID();
			ShapeTypes type2 = m_actors[inner]->getShapeID();

			CollisionUtils::checkCollision(m_actors[outer], m_actors[inner]);

			// fn collisionFunction = collisionFunctionArray[(int)type1 * ShapeTypes::SHAPECOUNT + (int)type2];

			//collisionFunction(m_actors[outer], m_actors[inner]);
		}
	}
}


// ---------------------------------------------------------
// colision detection functions
// ---------------------------------------------------------

// test collision between 2 planes
//bool PhysicsScene::plane2Plane(PhysicsObject* a, PhysicsObject* b)
//{
//	// planes never "collide" so just return false
//	return false;
//}
//
//// test collision between a plane and a sphere
//bool PhysicsScene::plane2Sphere(PhysicsObject* a, PhysicsObject* b)
//{
//	return sphere2Plane(b, a);
//}
//
//// test collision between a plane and a polygon
//bool PhysicsScene::plane2Polygon(PhysicsObject* a, PhysicsObject* b)
//{
//	return polygon2Plane(b, a);
//}
//
//// test collision between a sphere and a plane
//bool PhysicsScene::sphere2Plane(PhysicsObject* a, PhysicsObject* b)
//{
//	Sphere* sphere = dynamic_cast<Sphere*>(a);
//	Plane* plane = dynamic_cast<Plane*>(b);
//
//	//if we are successful then test for collision
//	if (sphere != nullptr && plane != nullptr)
//	{
//		// store planes normal
//		glm::vec2 planeNormal = plane->getNormal();
//
//		// get dot product of sphere position and plane normal
//		float sphereToPlane = glm::dot(
//			sphere->getPosition(),
//			planeNormal);
//
//		// subtract planes distance
//		sphereToPlane -= plane->getDistance();
//
//		//// if we are behind plane then we flip the normal
//		//if (sphereToPlane < 0)
//		//{
//		//	collisionNormal *= -1;
//		//	sphereToPlane *= -1;
//		//}
//
//		//float intersection = sphere->getRadius() - sphereToPlane;
//
//		if (fabsf(sphereToPlane) < sphere->getRadius())
//		{
//			//glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
//
//			// contact force
//			//sphere->setPosition(sphere->getPosition() +
//				//collisionNormal * (sphere->getRadius() - sphereToPlane));
//
//			//plane->resolveCollision(sphere, contact);
//
//			sphere->setVelocity(glm::vec2(0));
//			sphere->setAngularVelocity(0);
//
//			return true;
//		}
//	}
//	return false;
//}
//
//// test collision between 2 spheres
//bool PhysicsScene::sphere2Sphere(PhysicsObject* a, PhysicsObject* b)
//{
//	Sphere* sphere1 = dynamic_cast<Sphere*>(a);
//	Sphere* sphere2 = dynamic_cast<Sphere*>(b);
//
//	if (sphere1 != nullptr && sphere2 != nullptr)
//	{
//		// vector between the 2 spheres
//		glm::vec2 delta = sphere2->getPosition() - sphere1->getPosition();
//
//		// squared distance of delta
//		float squaredDistance = delta.x * delta.x + delta.y * delta.y;
//
//		// combined radii of spheres
//		float radius = sphere1->getRadius() + sphere2->getRadius();
//
//		// square radius
//		radius *= radius;
//
//		if (squaredDistance <= radius)
//		{
//			//glm::vec2 contactForce = 0.5f * (distance - (sphere1->getRadius() +
//				//sphere2->getRadius())) * delta / distance;
//
//			//sphere1->setPosition(sphere1->getPosition() + contactForce);
//			//sphere2->setPosition(sphere2->getPosition() - contactForce);
//
//			// respond to the collision
//			//sphere1->resolveCollision(sphere2, 0.5f * (sphere1->getPosition() +
//				//sphere2->getPosition()));
//
//			sphere1->setVelocity(glm::vec2(0));
//			sphere2->setVelocity(glm::vec2(0));
//
//			sphere1->setAngularVelocity(0);
//			sphere2->setAngularVelocity(0);
//
//
//			return true;
//		}
//	}
//
//
//	return false;
//}
//
//// test collision between a sphere and a polygon
//bool PhysicsScene::sphere2Polygon(PhysicsObject* a, PhysicsObject* b)
//{
//	return polygon2Sphere(b, a);
//}
//
//// test collision between a polygon and a plane
//bool PhysicsScene::polygon2Plane(PhysicsObject* a, PhysicsObject* b)
//{
//	phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(a);
//	Plane* plane = dynamic_cast<Plane*>(b);
//
//	if (polygon != nullptr && plane != nullptr)
//	{
//		return phy::Polygon::checkCollisionPlane(polygon, plane);
//	}
//
//	return false;
//}
//
//// test collision between a polygon and a sphere
//bool PhysicsScene::polygon2Sphere(PhysicsObject* a, PhysicsObject* b)
//{
//	phy::Polygon* polygon = dynamic_cast<phy::Polygon*>(a);
//	Sphere* sphere = dynamic_cast<Sphere*>(b);
//
//	if (polygon != nullptr && sphere != nullptr)
//	{
//		return phy::Polygon::checkCollisionSphere(polygon, sphere);
//	}
//
//	return false;
//}
//
//// test collision between 2 polygons
//bool PhysicsScene::polygon2Polygon(PhysicsObject* a, PhysicsObject* b)
//{
//	phy::Polygon* polygon1 = dynamic_cast<phy::Polygon*>(a);
//	phy::Polygon* polygon2 = dynamic_cast<phy::Polygon*>(b);
//
//	if (polygon1 != nullptr && polygon2 != nullptr)
//	{
//		return phy::Polygon::checkCollisionPolygon(polygon1, polygon2);
//	}
//
//	return false;
//}