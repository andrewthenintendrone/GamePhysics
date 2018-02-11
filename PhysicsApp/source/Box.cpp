#include "Box.h"
#include <Gizmos.h>
#include <iostream>

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, glm::vec4 color) :
	RigidBody(BOX, position, velocity, 0, mass)
{
	m_width = width;
	m_height = height;
	m_color = color;
}

void Box::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, glm::vec2(m_width, m_height), m_color);
}

bool Box::checkCollision(PhysicsObject* other)
{
	return false;
}

glm::vec2 Box::getCorner(int corner)
{
	glm::vec2 cornerPos = m_position;

	switch (corner)
	{
	case 1:
		cornerPos.x -= m_width;
		cornerPos.y += m_height;
		break;
	case 2:
		cornerPos.x += m_width;
		cornerPos.y += m_height;
		break;
	case 3:
		cornerPos.x -= m_width;
		cornerPos.y -= m_height;
		break;
	case 4:
		cornerPos.x += m_width;
		cornerPos.y -= m_height;
		break;
	default:
		std::cout << "Tried to access corner " << corner << " of a box.\n";
		break;
	}
	return cornerPos;
}
