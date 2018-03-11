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
	const float getTimeStep() { return m_timeStep; }

protected:

	void checkCollisions();

	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<phy::RigidBody*>m_actors;
};