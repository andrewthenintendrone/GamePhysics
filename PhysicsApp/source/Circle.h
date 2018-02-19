#pragma once
#include "RigidBody.h"
#include <glm\vec4.hpp>

#include "Renderer2D.h"

class Circle : public RigidBody
{
public:
	Circle(glm::vec2 position = glm::vec2(0, 0), glm::vec2 velocity = glm::vec2(0, 0),
		float mass = 1, float radius = 1, glm::vec4 color = glm::vec4(1));

	~Circle();
	virtual void makeGizmo();

	float getRadius() { return m_radius; }

protected:
	float m_radius;
	glm::vec4 m_color;
};