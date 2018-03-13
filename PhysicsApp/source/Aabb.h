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

	glm::vec2 getCorner(const int corner) const;
	bool containsPoint(const glm::vec2 point) const;

	void setExtents(const glm::vec2 extents) { m_extents = extents; }
	void setExtents(const float x, const float y) { m_extents = glm::vec2(x * 0.5f, y * 0.5f); }
	void setWidth(const float width) { m_extents.x = width * 0.5f; }
	void setHeight(const float height) { m_extents.y = height * 0.5f; }

protected:

	glm::vec2 m_extents = glm::vec2(0);
};