#pragma once
#include "RigidBody.h"
#include <glm\ext.hpp>

class Box : public RigidBody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, glm::vec4 color);
	~Box() {};

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* other);

	float getWidth() { return m_width; }
	float getHeight() { return m_height; }

	glm::vec2 getCorner(int corner);
	bool containsPoint(glm::vec2 point);

protected:
	float m_width;
	float m_height;
	glm::vec4 m_color;

};