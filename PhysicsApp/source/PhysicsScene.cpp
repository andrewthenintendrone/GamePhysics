#include "PhysicsScene.h"
#include <algorithm>
#include <list>
#include "RigidBody.h"
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <glm\ext.hpp>

// function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box
};

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

void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	m_actors.erase(std::remove(m_actors.begin(), m_actors.end(), actor), m_actors.end());
}

void PhysicsScene::update(float dt)
{
	static std::list<PhysicsObject*> dirty;

	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		checkForCollison();
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::checkForCollison()
{
	int actorCount = m_actors.size();

	// need to check for collisions against all objects except this one
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			// using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did the collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool PhysicsScene::plane2Plane(PhysicsObject* a, PhysicsObject* b)
{
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* a, PhysicsObject* b)
{
	return sphere2Plane(b, a);
}

bool PhysicsScene::plane2Box(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to sphere and plane
	Sphere* sphere = dynamic_cast<Sphere*>(a);
	Plane* plane = dynamic_cast<Plane*>(b);

	// if we are successful then check for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(
			sphere->getPosition(),
			plane->getNormal()) - plane->getDistance();

		// if we are behind the plane then we flip the normal
		if (sphereToPlane <= 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection >= 0)
		{
			// set sphere velocity to 0
			sphere->setVelocity(glm::vec2(0, 0));
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(a);
	Sphere* sphere2 = dynamic_cast<Sphere*>(b);

	// if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		if (glm::distance(sphere1->getPosition(), sphere2->getPosition()) <= (sphere1->getRadius() + sphere2->getRadius()))
		{
			// collision has occurred
			// TODO: collision resolution
			sphere1->setVelocity(glm::vec2(0, 0));
			sphere2->setVelocity(glm::vec2(0, 0));

			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject* a, PhysicsObject* b)
{
	return box2Sphere(b, a);
}

bool PhysicsScene::box2Plane(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to box and plane
	Box* box = dynamic_cast<Box*>(a);
	Plane* plane = dynamic_cast<Plane*>(b);

	// if we are successful then test for collision
	if (box != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();

		float cornerDistances[4];
		bool signs[4];

		// store the distance of each corner from the plane
		for (int i = 0; i < 4; i++)
		{
			cornerDistances[i] = glm::dot(
				box->getCorner(i + 1),
				plane->getNormal()) - plane->getDistance();

			// also store the signs of the distances
			signs[i] = std::signbit(cornerDistances[i]);
		}

		// if the sign of two opposite sides are different there is a collision
		if(signs[0] != signs[3] || signs[1] != signs[2])
		{
			box->setVelocity(glm::vec2(0, 0));
			return true;
		}
	}
	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to box and plane
	Box* box = dynamic_cast<Box*>(a);
	Sphere* sphere = dynamic_cast<Sphere*>(b);

	// if we are successful then test for collision
	if (box != nullptr && sphere != nullptr)
	{
		// find the closest point on the sphere to the box
		glm::vec2 collisionPoint = sphere->getPosition() + glm::normalize(box->getPosition() - sphere->getPosition()) * sphere->getRadius();

		// test if it is with the bounds of the box
		if (box->containsPoint(collisionPoint))
		{
			box->setVelocity(glm::vec2(0, 0));
			sphere->setVelocity(glm::vec2(0, 0));
			return true;
		}
	}
	return false;
}

bool PhysicsScene::box2Box(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to box and box
	Box* box1 = dynamic_cast<Box*>(a);
	Box* box2 = dynamic_cast<Box*>(b);

	// if we are successful then test for collision
	if (box1 != nullptr && box2 != nullptr)
	{
		bool test1 = (box1->getCorner(4).x < box2->getCorner(1).x);
		bool test2 = (box2->getCorner(4).x < box1->getCorner(1).x);
		bool test3 = (box1->getCorner(4).y > box2->getCorner(1).y);
		bool test4 = (box2->getCorner(4).y > box1->getCorner(1).y);

		if (!(test1 || test2 || test3 || test4))
		{
			box1->setVelocity(glm::vec2(0, 0));
			box2->setVelocity(glm::vec2(0, 0));
			return true;
		}
	}
	return false;
}