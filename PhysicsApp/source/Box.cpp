#include "Box.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#include <iostream>

Box::Box() :
	RigidBody(BOX)
{
	
}

// check if any of the other box's corners are inside this box
bool Box::checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts,
	glm::vec2& edgeNormal, glm::vec2& contactForce)
{
	float boxW = box.getExtents().x * 2;
	float boxH = box.getExtents().y * 2;
	float penetration = 0;

	for (float x = -box.getExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{
			// pos in worldspace
			glm::vec2 p = box.m_position + x * box.m_localX + y * box.m_localY;

			// position in our box's space
			glm::vec2 p0(glm::dot(p - m_position, m_localX),
				glm::dot(p - m_position, m_localY));

			float w2 = m_extents.x, h2 = m_extents.y;
			if (p0.y < h2 && p0.y > -h2)
			{
				if (p0.x > 0 && p0.x < w2)
				{
					numContacts++;
					contact += m_position + w2 * m_localX + p0.y * m_localY;
					edgeNormal = m_localX;
					penetration = w2 - p0.x;
				}
				if (p0.x < 0 && p0.x > -w2)
				{
					numContacts++;
					contact += m_position - w2 * m_localX + p0.y * m_localY;
					edgeNormal = -m_localX;
					penetration = w2 + p0.x;
				}
			}
			if (p0.x < w2 && p0.x > -w2)
			{
				if (p0.y > 0 && p0.y < h2)
				{
					numContacts++;
					contact += m_position + p0.x * m_localX + h2 * m_localY;
					float pen0 = h2 - p0.y;
					if (pen0 < penetration || penetration == 0)
					{
						penetration = pen0;
						edgeNormal = m_localY;
					}
				}
				if (p0.y < 0 && p0.y > -h2)
				{
					numContacts++;
					contact += m_position + p0.x * m_localX - h2 * m_localY;
					float pen0 = h2 + p0.y;
					if (pen0 < penetration || penetration == 0)
					{
						penetration = pen0;
						edgeNormal = -m_localY;
					}
				}
			}
		}
	}
	contactForce = penetration * edgeNormal;
	return (penetration != 0);
}


void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);

	// store the local axis
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::vec2(cs, sn);
	m_localY = glm::vec2(-sn, cs);
}

void Box::draw()
{
	// if only using rotation
	// glm::mat4 transform = glm::rotate(m_rotation, glm::vec3(0, 0, 1));
	// aie::Gizmos::add2DAABBFilled(getCenter(),
	// m_extents, m_colour, &transform);
	// draw using local axes
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_color);
	aie::Gizmos::add2DTri(p1, p4, p3, m_color);
}

// calculate moment of inertia
void Box::calculateMoment()
{
	m_moment = 1.0f / 12.0f * m_mass * (m_extents.x * 2) * (m_extents.y * 2);
}
