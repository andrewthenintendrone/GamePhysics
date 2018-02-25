#include "Polygon.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#define _USE_MATH_DEFINES
#include <math.h>

abc::Polygon::Polygon(std::vector<glm::vec2> points) :
	RigidBody(POLYGON, glm::vec2(0), glm::vec2(0), 0, 1)
{
	m_points = points;
	m_numFaces = m_points.size();

	calculateNormals();
}

// calculate normals of points
void abc::Polygon::calculateNormals()
{
	for (int i = 0; i < m_points.size(); i++)
	{
		if (i == m_points.size() - 1)
		{
			// find vector between start and end point
			glm::vec2 perp = m_points[0] - m_points[i];

			// calculate normal from perpendicular
			glm::vec2 normal = glm::normalize(glm::vec2(perp.y, -perp.x));

			// add normal to the list
			m_normals.push_back(normal);
		}
		else
		{
			// find vector between the two points
			glm::vec2 perp = m_points[i + 1] - m_points[i];

			// calculate normal from perpendicular
			glm::vec2 normal = glm::normalize(glm::vec2(perp.y, -perp.x));

			// add normal to the list
			m_normals.push_back(normal);
		}
	}
}

void abc::Polygon::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKinematic)
	{
		return;
	}

	// remember when applying the force of gravity, mass cancels out
	m_velocity += gravity * timeStep;
	m_position += m_velocity * timeStep;

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_angularVelocity * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		if (length(m_velocity) < length(gravity) * m_linearDrag * timeStep)
		{
			m_velocity = glm::vec2(0);
		}
	}

	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
	{
		m_angularVelocity = 0;
	}
}

void abc::Polygon::makeGizmo()
{
	glm::vec4 cyan(0, 1, 1, 1);
	glm::vec2 faceCenter;

	for (int i = 0; i < m_points.size(); i++)
	{
		if (i == m_points.size() - 1)
		{
			aie::Gizmos::add2DTri(m_position, m_position + m_points[i], m_position + m_points[0], m_color);

			faceCenter = (m_position + m_points[i] + m_position + m_points[0]) * 0.5f;
		}
		else
		{
			aie::Gizmos::add2DTri(m_position, m_position + m_points[i], m_position + m_points[i + 1], m_color);

			faceCenter = (m_position + m_points[i] + m_position + m_points[i + 1]) * 0.5f;
		}

		// draw normals in cyan
		aie::Gizmos::add2DLine(faceCenter, faceCenter + m_normals[i] * 16.0f, cyan);
	}
}
