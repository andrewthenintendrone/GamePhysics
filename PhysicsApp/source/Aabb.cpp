#include "AABB.h"
#include <Gizmos.h>
#include <iostream>
#include <glm\ext.hpp>

Aabb::Aabb(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, glm::vec4 color) :
	RigidBody(ShapeTypes::AABB, position, velocity, 0, mass)
{
	m_extents = extents;
	m_color = color;
	m_moment = 1.0f / 6.0f * m_mass * (extents.x * 2) * (m_extents.y * 2);
}

// adds an AIE Gizmo to the scene
void Aabb::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_extents, m_color);
}

bool Aabb::checkBoxCorners(const Aabb& box, glm::vec2& contact, int& numContacts, glm::vec2& edgeNormal, glm::vec2& contactForce)
{
	float boxW = box.getExtents().x * 2;
	float boxH = box.getExtents().y * 2;
	float penetration = 0;

	for (float x = -box.getExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{
			// pos in worldspace
			glm::vec2 p = box.m_position + x + y;
			// position in our box's space
			glm::vec2 p0(glm::dot(p - m_position, p - m_position));

			float w2 = m_extents.x, h2 = m_extents.y;

			if (p0.y < h2 && p0.y > -h2)
			{
				if (p0.x > 0 && p0.x < w2)
				{
					numContacts++;
					contact += m_position + w2 + p0.y;
					edgeNormal = glm::vec2(1, 0);
					penetration = w2 - p0.x;
				}
				if (p0.x < 0 && p0.x > -w2)
				{
					numContacts++;
					contact += m_position - w2 + p0.y;
					edgeNormal = glm::vec2(-1, 0);
					penetration = w2 + p0.x;
				}
			}
			if (p0.x < w2 && p0.x > -w2)
			{
				if (p0.y > 0 && p0.y < h2)
				{
					numContacts++;
					contact += m_position + p0.x + h2;
					float pen0 = h2 - p0.y;
					if (pen0 < penetration || penetration == 0)
					{
						penetration = pen0;
						edgeNormal = glm::vec2(0, 1);
					}
				}
				if (p0.y < 0 && p0.y > -h2)
				{
					numContacts++;
					contact += m_position + p0.x - h2;
					float pen0 = h2 + p0.y;
					if (pen0 < penetration || penetration == 0)
					{
						penetration = pen0;
						edgeNormal = glm::vec2(0, -1);
					}
				}
			}
		}
	}
	contactForce = penetration * edgeNormal;
	return (penetration != 0);
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
