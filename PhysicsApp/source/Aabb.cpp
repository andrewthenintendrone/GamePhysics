#include "AABB.h"
#include <Gizmos.h>
#include <iostream>
#include <glm\ext.hpp>

Aabb::Aabb() :
	RigidBody(ShapeTypes::AABB)
{
	// m_moment = 1.0f / 6.0f * m_mass * (extents.x * 2) * (m_extents.y * 2);
}

// adds an AIE Gizmo to the scene
void Aabb::draw()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_extents, m_color);
}

// returns the corner position
glm::vec2 Aabb::getCorner(int corner)
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

bool Aabb::containsPoint(glm::vec2 point)
{
	if (point.x > m_position.x + m_extents.x || point.x < m_position.x - m_extents.x)
	{
		return false;
	}
	if (point.y > m_position.y + m_extents.y || point.y < m_position.y - m_extents.y)
	{
		return false;
	}
	return true;
}
