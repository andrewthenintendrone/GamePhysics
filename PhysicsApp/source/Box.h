#pragma once
#include "RigidBody.h"
#include <glm\common.hpp>

class Box : public RigidBody
{
public:
	Box();
	~Box() {};

	glm::vec2 getExtents() const { return m_extents; }

	float getWidth() const { return m_extents.x * 2; }
	float getHeight() const { return m_extents.y * 2; }

	glm::vec2 getLocalX() const { return m_localX; }
	glm::vec2 getLocalY() const { return m_localY; }

	void setExtents(const glm::vec2 extents) { m_extents = extents; }
	void setExtents(const float x, const float y) { m_extents = glm::vec2(x * 0.5f, y * 0.5f); }
	void setWidth(const float width) { m_extents.x = width * 0.5f; }
	void setHeight(const float height) { m_extents.y = height * 0.5f; }

	bool Box::checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts,
		glm::vec2& edgeNormal, glm::vec2& contactForce);

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void draw();

	void calculateMoment();

protected:
	glm::vec2 m_extents = glm::vec2(1, 1); // the halfedge lengths

	// store the local x, y axes of the box based on its angle of rotation
	glm::vec2 m_localX = glm::vec2(0);
	glm::vec2 m_localY = glm::vec2(0);
};