#include "Polygon.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "CollisionUtils.h"

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

	std::vector<glm::vec2> getSupports(Polygon* polygon, glm::vec2 collisionNormal)
	{
		// if we are dealing with a circle
		// points should be length 2
		// first point is circle poisition + collision normal * circle radius
		// second point is circle position - collision normal * circle radius
		std::vector<glm::vec2> points = polygon->getLocalPointsInWorldSpace();

		float maxDist = FLT_MAX;
		glm::vec2 support;

		glm::vec2 v2;
		bool hasv2 = false;

		for (auto iter = points.begin(); iter != points.end(); iter++)
		{
			float dist = glm::dot(*iter, collisionNormal);

			if (CollisionUtils::equal(dist, maxDist))
			{
				hasv2 = true;
				maxDist = dist;
				v2 = *iter;
			}
			else if (dist < maxDist)
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

	// return whether the polygon contains a point
	bool Polygon::containsPoint(glm::vec2 point)
	{
		// get all points on the polygon
		std::vector<glm::vec2> points = getLocalPointsInWorldSpace();

		bool contains = false;

		for (int i = 0, j = points.size() - 1; i < points.size(); j = i++)
		{
			if (((points[i].y > point.y) != (points[j].y > point.y)) &&
				(point.x < (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x))
			{
				contains = !contains;
			}
		}
		return contains;
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
	bool Polygon::checkCollisionPolygon(Polygon* polygon1, Polygon* polygon2)
	{
		// do a quick AABB check before doing
		// any expensive calculations
		if (!checkCollisionAABB(polygon1, polygon2))
		{
			return false;
		}

		std::vector<glm::vec2> points1 = polygon1->getLocalPointsInWorldSpace();
		std::vector<glm::vec2> points2 = polygon2->getLocalPointsInWorldSpace();

		std::vector<glm::vec2> normals1 = polygon1->getLocalNormals();
		std::vector<glm::vec2> normals2 = polygon2->getLocalNormals();

		// combine all normals
		std::vector<glm::vec2> allNormals = normals1;
		allNormals.insert(allNormals.end(), normals2.begin(), normals2.end());

		float minimumPenetration = FLT_MAX;
		glm::vec2 collisionNormal(0);

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
				Projection currentProjection(points1[j], allNormals[i]);

				if (currentProjection.dotAlongNormal <= minA)
				{
					minA = currentProjection.dotAlongNormal;
				}
				if (currentProjection.dotAlongNormal >= maxA)
				{
					maxA = currentProjection.dotAlongNormal;
				}
			}
			// for each other point
			for (int j = 0; j < points2.size(); j++)
			{
				// project other point onto normal
				Projection currentProjection(points2[j], allNormals[i]);

				if (currentProjection.dotAlongNormal <= minB)
				{
					minB = currentProjection.dotAlongNormal;
				}
				if (currentProjection.dotAlongNormal >= maxB)
				{
					maxB = currentProjection.dotAlongNormal;
				}
			}

			float dist = CollisionUtils::getProjectionOverlap(minA, maxA, minB, maxB);

			// shadows missed there is no collision
			if (dist < 0)
			{
				return false;
			}

			if (dist < minimumPenetration)
			{
				minimumPenetration = dist;
				collisionNormal = allNormals[i];
			}
		}

		//aie::Gizmos::add2DLine(glm::vec2(0), glm::normalize(collisionNormal) * 8.0f, glm::vec4(1));

		// now to find the contact point
		//glm::vec2 contactPoint(0);

		//// one of the collision normals needs to be flipped
		//// figure out which one
		//float flipFirst = (std::signbit(glm::dot(polygon1->getPosition(), collisionNormal)) == 1 ? 1.0f : -1.0f);

		//// get support point(s)
		//std::vector<glm::vec2> supports1 = getSupports(polygon1, collisionNormal * flipFirst);
		//std::vector<glm::vec2> supports2 = getSupports(polygon2, collisionNormal * -flipFirst);

		//if (supports1.size() == 1)
		//{
		//	contactPoint = supports1[0];
		//}
		//else if (supports2.size() == 1)
		//{
		//	contactPoint = supports2[0];
		//}
		//else
		//{
		//	// both supports have 2 points
		//	// find the average of the middle two points
		//	// this is the contact point
		//	std::vector<Projection> projectionsAlongNormal;

		//	projectionsAlongNormal.push_back(Projection(supports1[0], glm::vec2(collisionNormal.y, -collisionNormal.x)));
		//	projectionsAlongNormal.push_back(Projection(supports1[1], glm::vec2(collisionNormal.y, -collisionNormal.x)));
		//	projectionsAlongNormal.push_back(Projection(supports2[0], glm::vec2(collisionNormal.y, -collisionNormal.x)));
		//	projectionsAlongNormal.push_back(Projection(supports2[1], glm::vec2(collisionNormal.y, -collisionNormal.x)));

		//	// sort by dot along collision normal
		//	std::sort(projectionsAlongNormal.begin(), projectionsAlongNormal.end(), CollisionUtils::sortByDot);

		//	contactPoint = 0.5f * (projectionsAlongNormal[1].point + projectionsAlongNormal[2].point);
		//}

		//aie::Gizmos::add2DCircle(contactPoint, 10, 36, glm::vec4(1, 1, 0, 1));

		//polygon1->correctPosition(polygon2, minimumPenetration, &collisionNormal);
		//polygon1->resolveCollision(polygon2, contactPoint, &collisionNormal);

		polygon1->setVelocity(glm::vec2(0));
		polygon2->setVelocity(glm::vec2(0));

		polygon1->setAngularVelocity(0);
		polygon2->setAngularVelocity(0);

		return true;
	}

	// check collision between a Polygon and a Sphere
	bool Polygon::checkCollisionSphere(Polygon* polygon, Sphere* sphere)
	{
		if (polygon->containsPoint(sphere->getPosition()))
		{
			polygon->setVelocity(glm::vec2(0));
			polygon->setAngularVelocity(0);

			sphere->setVelocity(glm::vec2(0));
			sphere->setAngularVelocity(0);

			return true;
		}

		return false;
	}

	// check collision between a Polygon and a plane
	bool Polygon::checkCollisionPlane(Polygon* polygon, Plane* plane)
	{
		// cast all points onto the plane
		std::vector<glm::vec2> points = polygon->getLocalPointsInWorldSpace();

		glm::vec2 planeNormal = plane->getNormal();

		Projection minA{ glm::vec2(0), FLT_MAX };
		Projection maxA{ glm::vec2(0), -FLT_MAX };

		for (auto iter = points.begin(); iter != points.end(); iter++)
		{
			// project point onto normal
			Projection currentProjection{ *iter, planeNormal };
			currentProjection.dotAlongNormal -= plane->getDistance();

			if (currentProjection.dotAlongNormal <= minA.dotAlongNormal)
			{
				minA = currentProjection;
			}
			if (currentProjection.dotAlongNormal >= maxA.dotAlongNormal)
			{
				maxA = currentProjection;
			}
		}

		//aie::Gizmos::add2DLine(minA.point, minA.point - planeNormal * minA.dotAlongNormal, glm::vec4(0, 1, 0, 1));
		//aie::Gizmos::add2DLine(maxA.point, maxA.point - planeNormal * maxA.dotAlongNormal, glm::vec4(1, 0, 0, 1));

		if (std::signbit(minA.dotAlongNormal) == std::signbit(maxA.dotAlongNormal))
		{
			return false;
		}

		// now to find the contact point
		//glm::vec2 contactPoint(0);

		// one of the collision normals needs to be flipped
		// figure out which one
		//float flipFirst = (std::signbit(glm::dot(polygon->getPosition(), planeNormal)) == 1 ? 1.0f : -1.0f);

		//std::vector<glm::vec2> supports = getSupports(polygon, planeNormal * flipFirst);

		/*if (supports.size() == 1)
		{
			contactPoint = supports[0];
		}
		else if(supports.size() == 2)
		{
			contactPoint = 0.5f * (supports[0] + supports[1]);
		}*/

		//aie::Gizmos::add2DCircle(contactPoint, 8, 12, glm::vec4(0, 1, 1, 1));

		//polygon->correctPosition(plane, minA.dotAlongNormal, &planeNormal);
		//plane->resolveCollision(polygon, contactPoint);

		polygon->setVelocity(glm::vec2(0));
		polygon->setAngularVelocity(0);

		return true;
	}
}
