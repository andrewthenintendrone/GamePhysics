#pragma once
#include <glm\vec4.hpp>
#include "RigidBody.h"

class Plane : public RigidBody
{
public:
	Plane();
	Plane(glm::vec2 normal, float distance);
	~Plane() {};

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {};
	virtual void makeGizmo();

	glm::vec2 getPosition() { return m_distanceToOrigin * m_normal; }
	glm::vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
};