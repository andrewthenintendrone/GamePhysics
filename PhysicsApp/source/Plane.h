#pragma once
#include <glm\glm.hpp>
#include "RigidBody.h"

class Plane : public RigidBody
{
public:
	Plane();
	~Plane() {};

	virtual void draw();
	virtual void resetPosition();

	glm::vec2 getPosition() const { return m_distance * m_normal; }
	glm::vec2 getNormal() const { return m_normal; }
	float getDistance() const { return m_distance; }

	void setNormal(const glm::vec2 normal) { m_normal = glm::normalize(normal); }
	void setNormal(const float x, const float y) { m_normal = glm::normalize(glm::vec2(x, y)); }
	void setDistance(const float distance) { m_distance = distance; }


	void resolveCollision(RigidBody* actor2, glm::vec2 contact);

protected:
	glm::vec2 m_normal = glm::vec2(0, 1);
	float m_distance = 0;
};