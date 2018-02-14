#pragma once
#include "RigidBody.h"
#include <glm\ext.hpp>

class Box : public RigidBody
{
public:
	Box(glm::vec2 position = glm::vec2(0), glm::vec2 velocity = glm::vec2(0), float mass = 1, glm::vec2 extents = glm::vec2(1), glm::vec4 color = glm::vec4(1));
	~Box() {};

	glm::vec2 getExtents() const { return m_extents; }
	glm::vec2 getCenter() const;

	glm::vec2 getLocalX() const { return m_localX; }
	glm::vec2 getLocalY() const { return m_localY; }

	float getWidth() { return m_extents.x * 2; }
	float getHeight() { return m_extents.y * 2; }

	//glm::vec2 getCorner(int corner);
	bool checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts,
		glm::vec2& edgeNormal, glm::vec2& contactForce);

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void makeGizmo();

	bool containsPoint(glm::vec2 point);

protected:
	glm::vec2 m_extents = glm::vec2(0, 0);
	glm::vec4 m_color;

	glm::vec2 m_localX = glm::vec2(0);
	glm::vec2 m_localY = glm::vec2(0);
};