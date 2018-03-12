#pragma once
#include "RigidBody.h"
#include <glm\common.hpp>

// Axis Aligned Bounding Box
class Aabb : public RigidBody
{
public:
	Aabb();

	~Aabb() {};

	virtual void draw();

	glm::vec2 getExtents() const { return m_extents; }

	float getWidth() const { return (m_extents.x * 2); }
	float getHeight() const { return (m_extents.y * 2); }

	glm::vec2 getMin() const { return m_position - m_extents; }
	glm::vec2 getMax() const { return m_position + m_extents; }

	glm::vec2 getCorner(int corner);
	bool containsPoint(glm::vec2 point);

protected:

	glm::vec2 m_extents = glm::vec2(0);
};