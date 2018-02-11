#include "Box.h"
#include <Gizmos.h>

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
	glm::vec2 returnValue = m_position;

	switch (corner)
	{
	case 1:
		returnValue.x -= m_width;
		returnValue.y -= m_height;
		break;
	case 2:
		returnValue.x += m_width;
		returnValue.y -= m_height;
		break;
	case 3:
		returnValue.x -= m_width;
		returnValue.y += m_height;
		break;
	case 4:
		returnValue.x += m_width;
		returnValue.y += m_height;
		break;
	}

	return returnValue;
}
