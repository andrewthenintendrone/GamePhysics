#include "Box.h"
#include <Gizmos.h>
#include <iostream>

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, glm::vec4 color) :
	RigidBody(BOX, position, velocity, 0, mass)
{
	m_extents = extents;
	m_color = color;
	m_moment = 1.0f / 12.0f * m_mass * (extents.x * 2) * (m_extents.y * 2);
}

glm::vec2 Box::getCorner(int corner)
{
	glm::vec2 cornerPos = m_position;

	switch (corner)
	{
	case 1:
		cornerPos.x -= m_extents.x;
		cornerPos.y += m_extents.y;
		break;
	case 2:
		cornerPos.x += m_extents.x;
		cornerPos.y += m_extents.y;
		break;
	case 3:
		cornerPos.x -= m_extents.x;
		cornerPos.y -= m_extents.y;
		break;
	case 4:
		cornerPos.x += m_extents.x;
		cornerPos.y -= m_extents.y;
		break;
	default:
		std::cout << "Tried to access corner " << corner << " of a box.\n";
		break;
	}

	return cornerPos;
}

void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);

	// store the local axis
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::makeGizmo()
{
	// draw using local axes
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_color);
	aie::Gizmos::add2DTri(p1, p4, p3, m_color);
}

// returns true if the point is within the box
bool Box::containsPoint(glm::vec2 point)
{
	return false;
}