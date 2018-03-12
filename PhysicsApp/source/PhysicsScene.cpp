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
		}
	}
}
