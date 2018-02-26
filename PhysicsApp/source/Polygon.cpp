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
	m_localPoints.resize(m_numFaces);

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
			m_localNormals.push_back(normal);
		}
		else
		{
			// find vector between the two points
			glm::vec2 perp = m_points[i + 1] - m_points[i];

			// calculate normal from perpendicular
			glm::vec2 normal = glm::normalize(glm::vec2(perp.y, -perp.x));

			// add normal to the list
			m_normals.push_back(normal);
			m_localNormals.push_back(normal);
		}
	}
}

// recalculates local points
void abc::Polygon::calculateLocalPoints()
{
	for (int i = 0; i < m_points.size(); i++)
	{
		float theta = glm::radians(m_rotation);

		float cs = cosf(theta);
		float sn = sinf(theta);

		glm::vec2 point = m_points[i];

		m_localPoints[i].x = point.x * cs - point.y * sn;
		m_localPoints[i].y = point.x * sn + point.y * cs;
	}
}

// recalculates local normals
void abc::Polygon::calculateLocalNormals()
{
	for (int i = 0; i < m_localNormals.size(); i++)
	{
		float theta = glm::radians(m_rotation);

		float cs = cosf(theta);
		float sn = sinf(theta);

		glm::vec2 normal = m_normals[i];

		m_localNormals[i].x = normal.x * cs - normal.y * sn;
		m_localNormals[i].y = normal.x * sn + normal.y * cs;
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

	calculateLocalPoints();
	calculateLocalNormals();
}

void abc::Polygon::makeGizmo()
{
	glm::vec2 faceCenter;

	std::vector<glm::vec2> localPoints;

	for (int i = 0; i < m_localPoints.size(); i++)
	{
		if (i == m_localPoints.size() - 1)
		{
			aie::Gizmos::add2DTri(m_position, m_position + m_localPoints[i], m_position + m_localPoints[0], m_color);

			faceCenter = (m_position + m_localPoints[i] + m_position + m_localPoints[0]) * 0.5f;
		}
		else
		{
			aie::Gizmos::add2DTri(m_position, m_position + m_localPoints[i], m_position + m_localPoints[i + 1], m_color);

			faceCenter = (m_position + m_localPoints[i] + m_position + m_localPoints[i + 1]) * 0.5f;
		}

		// draw normals in the opposite color
		aie::Gizmos::add2DLine(faceCenter, faceCenter + m_localNormals[i] * 16.0f, glm::vec4(1 - m_color.x, 1 - m_color.y, 1 - m_color.z, 1));
	}
}
