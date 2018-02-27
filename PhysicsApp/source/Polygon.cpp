#include "Polygon.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

namespace phy
{
	Polygon::Polygon(std::vector<glm::vec2> points) :
		RigidBody(POLYGON, glm::vec2(0), glm::vec2(0), 0, 1)
	{
		m_points = points;
		m_numFaces = m_points.size();
		m_localPoints.resize(m_numFaces);
		m_moment = 3;

		calculateNormals();
	}

	// calculate normals of points
	void Polygon::calculateNormals()
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
	void Polygon::calculateLocalPoints()
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
	void Polygon::calculateLocalNormals()
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

	void Polygon::fixedUpdate(glm::vec2 gravity, float timeStep)
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

	void Polygon::makeGizmo()
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

	std::vector<glm::vec2> Polygon::getLocalPointsInWorldSpace()
	{
		std::vector<glm::vec2> points;

		for (int i = 0; i < m_localPoints.size(); i++)
		{
			points.push_back(m_position + m_localPoints[i]);
		}

		return points;
	}

	bool checkContainment(const glm::vec2& proj1, const glm::vec2& proj2)
	{
		return (proj1.x < proj2.x && proj1.y > proj2.y);
	}

	float getOverlap(const glm::vec2& proj1,
		const glm::vec2& proj2)
	{
		return std::min(proj1.y, proj2.y) - std::max(proj1.x, proj2.x);
	}

	bool almostEqual(float a, float b)
	{
		return (abs(a - b) <= 0.00001f);
	}

	std::vector<glm::vec2> getSupports(Polygon* polygon, glm::vec2 collisionNormal)
	{
		// if we are dealing with a circle
		// points should be length 2
		// first point is circle poisition + collision normal * circle radius
		// second point is circle position - collision normal * circle radius
		std::vector<glm::vec2> points = polygon->getLocalPointsInWorldSpace();

		float maxDist = -FLT_MAX;
		glm::vec2 support;

		glm::vec2 v2;
		bool hasv2 = false;

		for (auto iter = points.begin(); iter != points.end(); iter++)
		{
			float dist = glm::dot(*iter, collisionNormal);

			if (almostEqual(dist, maxDist))
			{
				hasv2 = true;
				maxDist = dist;
				v2 = *iter;
			}
			else if (dist > maxDist)
			{
				hasv2 = false;
				maxDist = dist;
				support = *iter;
			}
		}

		if (hasv2)
		{
			return std::vector<glm::vec2>({ support, v2 });
		}
		else
		{
			return std::vector<glm::vec2>({ support });
		}
	}

	// returns the result of an Axis Aligned Bounding Box check between Polygons
	bool Polygon::checkCollisionAABB(Polygon* polygon1, Polygon* polygon2)
	{
		std::vector<glm::vec2> points1 = polygon1->getLocalPointsInWorldSpace();
		std::vector<glm::vec2> points2 = polygon2->getLocalPointsInWorldSpace();

		glm::vec2 aMin(FLT_MAX);
		glm::vec2 aMax(-FLT_MAX);

		glm::vec2 bMin(FLT_MAX);
		glm::vec2 bMax(-FLT_MAX);

		for (auto iter = points1.begin(); iter != points1.end(); iter++)
		{
			aMin.x = std::min(aMin.x, iter->x);
			aMin.y = std::min(aMin.y, iter->y);
			aMax.x = std::max(aMax.x, iter->x);
			aMax.y = std::max(aMax.y, iter->y);
		}

		for (auto iter = points2.begin(); iter != points2.end(); iter++)
		{
			bMin.x = std::min(bMin.x, iter->x);
			bMin.y = std::min(bMin.y, iter->y);
			bMax.x = std::max(bMax.x, iter->x);
			bMax.y = std::max(bMax.y, iter->y);
		}

		return (!(aMax.x < bMin.x || aMin.x > bMax.x || aMax.y < bMin.y || aMin.y > bMax.y));
	}

	// returns the result of a collision between Polygons
	glm::vec2 Polygon::checkCollision(Polygon* polygon1, Polygon* polygon2)
	{
		// do a quick AABB check before doing
		// any expensive calculations
		if (!checkCollisionAABB(polygon1, polygon2))
		{
			return glm::vec2(0);
		}

		std::vector<glm::vec2> points1 = polygon1->getLocalPointsInWorldSpace();
		std::vector<glm::vec2> points2 = polygon2->getLocalPointsInWorldSpace();

		std::vector<glm::vec2> normals1 = polygon1->getLocalNormals();
		std::vector<glm::vec2> normals2 = polygon2->getLocalNormals();

		// combine all normals
		std::vector<glm::vec2> allNormals = normals1;
		allNormals.insert(allNormals.end(), normals2.begin(), normals2.end());

		std::vector<float> shadow1;
		std::vector<float> shadow2;

		float minDist = FLT_MAX;
		glm::vec2 minPenetrationVector;

		// for each normal
		for (int i = 0; i < allNormals.size(); i++)
		{
			float minA = FLT_MAX;
			float minB = FLT_MAX;
			float maxA = -FLT_MAX;
			float maxB = -FLT_MAX;

			// for each point
			for (int j = 0; j < points1.size(); j++)
			{
				// project point onto normal
				float projectedPoint = glm::dot(allNormals[i], points1[j]);
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
			for (int j = 0; j < points2.size(); j++)
			{
				// project other point onto normal
				float projectedPoint = glm::dot(allNormals[i], points2[j]);
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

			float dist = getOverlap(glm::vec2(minA, maxA), glm::vec2(minB, maxB));

			// shadows missed there is no collision
			if (dist < 0)
			{
				return glm::vec2(0);
			}

			if (dist < minDist)
			{
				minDist = dist;
				minPenetrationVector = allNormals[i];
			}
		}

		polygon1->setVelocity(glm::vec2(0));
		polygon2->setVelocity(glm::vec2(0));

		// collision happened
		std::vector<glm::vec2> supports1 = getSupports(polygon1, -minPenetrationVector);
		std::vector<glm::vec2> supports2 = getSupports(polygon2, minPenetrationVector);

		for (auto iter = supports1.begin(); iter != supports1.end(); iter++)
		{
			aie::Gizmos::add2DCircle(*iter, 1, 8, glm::vec4(0, 1, 1, 1));
		}
		for (auto iter = supports2.begin(); iter != supports2.end(); iter++)
		{
			aie::Gizmos::add2DCircle(*iter, 1, 8, glm::vec4(0, 1, 1, 1));
		}

		// now to find the contact point
		glm::vec2 contact;

		// now we have the support point(s)
		if (supports1.size() == 1)
		{
			contact = supports1[0];
		}
		else if (supports2.size() == 1)
		{
			contact = supports2[0];
		}
		else
		{
			// both supports have 2 points

			// find the average of the middle two points
			// this is the contact point
			std::vector<float> pointsAlongCollisionNormal;

			pointsAlongCollisionNormal.push_back(glm::dot(supports1[0], minPenetrationVector));
			pointsAlongCollisionNormal.push_back(glm::dot(supports1[1], minPenetrationVector));
			pointsAlongCollisionNormal.push_back(glm::dot(supports2[0], minPenetrationVector));
			pointsAlongCollisionNormal.push_back(glm::dot(supports2[1], minPenetrationVector));




		}

		return minPenetrationVector;
	}
}
