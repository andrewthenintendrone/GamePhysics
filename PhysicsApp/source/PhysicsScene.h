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
	void update(const float dt);
	void draw();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	void setGravity(const float x, const float y) { m_gravity = glm::vec2(x, y); }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void checkForCollison();

	// collision detection funtions
	static bool plane2Plane(PhysicsObject* a, PhysicsObject* b);
	static bool plane2Sphere(PhysicsObject* a, PhysicsObject* b);
	static bool plane2Box(PhysicsObject* a, PhysicsObject* b);
	static bool plane2AABB(PhysicsObject* a, PhysicsObject* b);
	static bool sphere2Plane(PhysicsObject* a, PhysicsObject* b);
	static bool sphere2Sphere(PhysicsObject* a, PhysicsObject* b);
	static bool sphere2Box(PhysicsObject* a, PhysicsObject* b);
	static bool sphere2AABB(PhysicsObject* a, PhysicsObject* b);
	static bool box2Plane(PhysicsObject* a, PhysicsObject* b);
	static bool box2Sphere(PhysicsObject* a, PhysicsObject* b);
	static bool box2Box(PhysicsObject* a, PhysicsObject* b);
	static bool box2AABB(PhysicsObject* a, PhysicsObject* b);
	static bool AABB2Plane(PhysicsObject* a, PhysicsObject* b);
	static bool AABB2Sphere(PhysicsObject* a, PhysicsObject* b);
	static bool AABB2Box(PhysicsObject* a, PhysicsObject* b);
	static bool AABB2AABB(PhysicsObject* a, PhysicsObject* b);

protected:

	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*>m_actors;
};