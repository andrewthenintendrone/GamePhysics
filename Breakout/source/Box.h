#pragma once
#include "RigidBody.h"
#include <glm\ext.hpp>

class Box : public RigidBody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, float length, float width, glm::vec4 color);
	~Box() {};

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* other);

	float getWidth() { return m_width; }
	float getHeight() { return m_height; }
	

protected:
	float m_width;
	float m_height;
	glm::vec4 m_color;

};