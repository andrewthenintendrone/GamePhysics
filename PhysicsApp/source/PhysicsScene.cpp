#include "PhysicsScene.h"
#include <algorithm>
#include <list>
#include "RigidBody.h"
#include <iostream>
#include "Circle.h"
#include "Plane.h"
#include "Box.h"
#include <glm\ext.hpp>

// function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane, PhysicsScene::plane2Circle, PhysicsScene::plane2Box,
	PhysicsScene::circle2Plane, PhysicsScene::circle2Circle, PhysicsScene::circle2Box,
	PhysicsScene::box2Plane, PhysicsScene::box2Circle, PhysicsScene::box2Box
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

bool PhysicsScene::plane2Circle(PhysicsObject* a, PhysicsObject* b)
{
	return circle2Plane(b, a);
}

bool PhysicsScene::plane2Box(PhysicsObject* a, PhysicsObject* b)
{
	return box2Plane(b, a);
}

bool PhysicsScene::circle2Plane(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to circle and plane
	Circle* circle = dynamic_cast<Circle*>(a);
	Plane* plane = dynamic_cast<Plane*>(b);

	// if we are successful then check for collision
	if (circle != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float circleToPlane = glm::dot(
			circle->getPosition(),
			plane->getNormal()) - plane->getDistance();

		// if we are behind the plane then we flip the normal
		if (circleToPlane <= 0)
		{
			collisionNormal *= -1;
			circleToPlane *= -1;
		}

		float intersection = circle->getRadius() - circleToPlane;
		if (intersection >= 0)
		{
			// collision
			glm::vec2 contact = circle->getPosition() - (collisionNormal * circle->getRadius());

			plane->resolveCollision(circle, contact);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::circle2Circle(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to sphere and sphere
	Circle* circle1 = dynamic_cast<Circle*>(a);
	Circle* circle2 = dynamic_cast<Circle*>(b);

	// if we are successful then test for collision
	if (circle1 != nullptr && circle2 != nullptr)
	{
		if (glm::distance(circle1->getPosition(), circle2->getPosition()) <= (circle1->getRadius() + circle2->getRadius()))
		{
			// collision
			circle1->resolveCollision(circle2, 0.5f * (circle1->getPosition() +
				circle2->getPosition()));

			return true;
		}
	}

	return false;
}

bool PhysicsScene::circle2Box(PhysicsObject* a, PhysicsObject* b)
{
	return box2Circle(b, a);
}

bool PhysicsScene::box2Plane(PhysicsObject* a, PhysicsObject* b)
{
	Box* box = dynamic_cast<Box*>(a);
	Plane* plane = dynamic_cast<Plane*>(b);

	if (box != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		glm::vec2 min = box->getMin();
		glm::vec2 max = box->getMax();
		float cornerDistances[4];

		cornerDistances[0] = glm::dot(
			glm::vec2(min.x, min.y),
			collisionNormal) - plane->getDistance();

		cornerDistances[1] = glm::dot(
			glm::vec2(max.x, min.y),
			collisionNormal) - plane->getDistance();

		cornerDistances[2] = glm::dot(
			glm::vec2(min.x, max.y),
			collisionNormal) - plane->getDistance();

		cornerDistances[3] = glm::dot(
			glm::vec2(max.x, max.y),
			collisionNormal) - plane->getDistance();

		bool signs[4];

		for (int i = 0; i < 4; i++)
		{
			signs[i] = std::signbit(cornerDistances[i]);
		}

		if (signs[0] != signs[3] || signs[1] != signs[2])
		{
			plane->resolveCollision(box, box->getPosition());
			return true;
		}
	}
	return false;
}

bool PhysicsScene::box2Circle(PhysicsObject* a, PhysicsObject* b)
{
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
		glm::vec2 aMin = box1->getMin();
		glm::vec2 aMax = box1->getMax();
		glm::vec2 bMin = box2->getMin();
		glm::vec2 bMax = box2->getMax();

		if (aMax.x < bMin.x || aMin.x > bMax.x)
		{
			return false;
		}
		if (aMax.y < bMin.y || aMin.y > bMax.y)
		{
			return false;
		}
		return true;
	}
	return false;
}