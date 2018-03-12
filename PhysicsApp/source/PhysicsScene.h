#pragma once
#include <glm\vec2.hpp>
#include <vector>
#include "RigidBody.h"
#include <algorithm>

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(phy::RigidBody* actor) { m_actors.push_back(actor); }
	void removeActor(phy::RigidBody* actor) { m_actors.erase(std::remove(m_actors.begin(), m_actors.end(), actor), m_actors.end()); }
	void update(float deltaTime);
	void draw();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	void setGravity(const float x = 0, const float y = 0) { m_gravity = glm::vec2(x, y); }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
<<<<<<< HEAD
	const float getTimeStep() { return m_timeStep; }
=======
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
>>>>>>> parent of 89a0f71... done changing circle back to sphere

protected:

	void checkCollisions();

	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<phy::RigidBody*>m_actors;
};