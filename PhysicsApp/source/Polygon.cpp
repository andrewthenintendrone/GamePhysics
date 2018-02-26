#include "Polygon.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

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
	// draw inverted if colliding
	glm::vec4 drawColor = m_isColliding ? glm::vec4(1 - m_color.x, 1 - m_color.y, 1 - m_color.z, 1) : m_color;

	// used to keep track of position of the center of each face
	glm::vec2 faceCenter;

	// iterate through local points
	for (int i = 0; i < m_localPoints.size(); i++)
	{
		// connect the last point to the first
		if (i == m_localPoints.size() - 1)
		{
			aie::Gizmos::add2DTri(m_position, m_position + m_localPoints[i], m_position + m_localPoints[0], drawColor);

			faceCenter = (m_position + m_localPoints[i] + m_position + m_localPoints[0]) * 0.5f;
		}
		// connect to the next point
		else
		{
			aie::Gizmos::add2DTri(m_position, m_position + m_localPoints[i], m_position + m_localPoints[i + 1], drawColor);

			faceCenter = (m_position + m_localPoints[i] + m_position + m_localPoints[i + 1]) * 0.5f;
		}
	}
}

std::vector<glm::vec2> abc::Polygon::getLocalPointsInWorldSpace()
{
	std::vector<glm::vec2> points;

	for (int i = 0; i < m_localPoints.size(); i++)
	{
		points.push_back(m_position + m_localPoints[i]);
	}

	return points;
}

// returns true if there is a collision between
// this Polygon and another
bool abc::Polygon::checkCollision(Polygon* other)
{
	// TODO: quick AABB test

	std::vector<glm::vec2> myPoints = getLocalPointsInWorldSpace();
	std::vector<glm::vec2> otherPoints = other->getLocalPointsInWorldSpace();
	std::vector<glm::vec2> otherNormals = other->getLocalNormals();

	// combine all normals
	std::vector<glm::vec2> allNormals = m_localNormals;
	allNormals.insert(allNormals.end(), otherNormals.begin(), otherNormals.end());

	std::vector<float> shadow1;
	std::vector<float> shadow2;

	float minA = FLT_MAX;
	float minB = FLT_MAX;
	float maxA = -FLT_MAX;
	float maxB = -FLT_MAX;

	// for each normal
	for (int i = 0; i < allNormals.size(); i++)
	{
		// for each point
		for (int j = 0; j < myPoints.size(); j++)
		{
			// project point onto normal
			float projectedPoint = glm::dot(allNormals[i], myPoints[j]);
			shadow1.push_back(projectedPoint);

			if (projectedPoint <= minA)
			{
				minA = projectedPoint;
			}
			if (projectedPoint >= maxA)
			{
				maxA = projectedPoint;
			}
		}
		// for each other point
		for (int j = 0; j < otherPoints.size(); j++)
		{
			// project other point onto normal
			float projectedPoint = glm::dot(allNormals[i], otherPoints[j]);
			shadow2.push_back(projectedPoint);

			if (projectedPoint <= minB)
			{
				minB = projectedPoint;
			}
			if (projectedPoint >= maxB)
			{
				maxB = projectedPoint;
			}
		}

		// shadows missed there is no collision
		if(minA > maxB || minB > maxA)
		{
			other->m_isColliding = false;
			m_isColliding = false;
			return false;
		}

		minA = FLT_MAX;
		minB = FLT_MAX;
		maxA = -FLT_MAX;
		maxB = -FLT_MAX;
	}

	// collision happened change color
	// to make it obvious
	other->m_isColliding = true;
	other->setVelocity(glm::vec2(0));
	other->setAngularVelocity(0);
	m_isColliding = true;
	setVelocity(glm::vec2(0));
	setAngularVelocity(0);
	return true;
}
