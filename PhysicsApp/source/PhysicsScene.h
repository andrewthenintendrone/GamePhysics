#pragma once
#include <glm\vec2.hpp>
#include <vector>
#include "PhysicsObject.h"

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float dt);
	void updateGizmos();
	void debugScene();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void checkForCollison();

	// collision detection funtions
	static bool plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool plane2Circle(PhysicsObject*, PhysicsObject*);
	static bool plane2Box(PhysicsObject*, PhysicsObject*);
	static bool plane2AABB(PhysicsObject*, PhysicsObject*);
	static bool circle2Plane(PhysicsObject*, PhysicsObject*);
	static bool circle2Circle(PhysicsObject*, PhysicsObject*);
	static bool circle2Box(PhysicsObject*, PhysicsObject*);
	static bool circle2AABB(PhysicsObject*, PhysicsObject*);
	static bool box2Plane(PhysicsObject*, PhysicsObject*);
	static bool box2Circle(PhysicsObject*, PhysicsObject*);
	static bool box2Box(PhysicsObject*, PhysicsObject*);
	static bool box2AABB(PhysicsObject*, PhysicsObject*);
	static bool AABB2Plane(PhysicsObject*, PhysicsObject*);
	static bool AABB2Circle(PhysicsObject*, PhysicsObject*);
	static bool AABB2Box(PhysicsObject*, PhysicsObject*);
	static bool AABB2AABB(PhysicsObject*, PhysicsObject*);

protected:

	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*>m_actors;
};