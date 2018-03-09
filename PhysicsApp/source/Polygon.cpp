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
		RigidBody(POLYGON)
	{
		m_points = points;
		m_localPoints.resize(m_points.size());
		m_moment = 1;

		calculateNormals();
	}

	// constructor (number of points and radius)
	Polygon::Polygon(int numPoints, float radius) :
		RigidBody(POLYGON)
	{
		for (int i = 0; i < numPoints; i++)
		{
			float theta = glm::radians(i * 360.0f / (float)numPoints);

			float sn = sinf(theta);
			float cs = cosf(theta);

			glm::vec2 currentPoint(sn, -cs);

			currentPoint *= -radius;

			m_points.push_back(currentPoint);
		}

		m_localPoints.resize(m_points.size());
		m_moment = 1;

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

	void Polygon::draw()
	{
		// iterate through local points
		for (int i = 0; i < m_localPoints.size(); i++)
		{
			// connect the last point to the first
			if (i == m_localPoints.size() - 1)
			{
				aie::Gizmos::add2DTri(m_position, m_position + m_localPoints[i], m_position + m_localPoints[0], m_color);
			}
			// connect to the next point
			else
			{
				aie::Gizmos::add2DTri(m_position, m_position + m_localPoints[i], m_position + m_localPoints[i + 1], m_color);
			}
		}
	}

	// Calculate bounding points for polygon
	AABBPoints Polygon::getBounds()
	{
		AABBPoints extents{ FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX };

		for (std::vector<glm::vec2>::iterator iter = m_localPoints.begin(); iter != m_localPoints.end(); iter++)
		{
			extents.left = std::min(extents.left, iter->x);
			extents.right = std::max(extents.right, iter->x);
			extents.top = std::max(extents.top, iter->y);
			extents.bottom = std::min(extents.bottom, iter->y);
		}

		return extents;
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
	//bool Polygon::checkCollisionAABB(Polygon* polygon1, Polygon* polygon2)
	//{
	//	std::vector<glm::vec2> points1 = polygon1->getLocalPointsInWorldSpace();
	//	std::vector<glm::vec2> points2 = polygon2->getLocalPointsInWorldSpace();

	//	glm::vec2 aMin(FLT_MAX);
	//	glm::vec2 aMax(-FLT_MAX);

	//	glm::vec2 bMin(FLT_MAX);
	//	glm::vec2 bMax(-FLT_MAX);

	//	for (auto iter = points1.begin(); iter != points1.end(); iter++)
	//	{
	//		aMin.x = std::min(aMin.x, iter->x);
	//		aMin.y = std::min(aMin.y, iter->y);
	//		aMax.x = std::max(aMax.x, iter->x);
	//		aMax.y = std::max(aMax.y, iter->y);
	//	}

	//	for (auto iter = points2.begin(); iter != points2.end(); iter++)
	//	{
	//		bMin.x = std::min(bMin.x, iter->x);
	//		bMin.y = std::min(bMin.y, iter->y);
	//		bMax.x = std::max(bMax.x, iter->x);
	//		bMax.y = std::max(bMax.y, iter->y);
	//	}

	//	return (!(aMax.x < bMin.x || aMin.x > bMax.x || aMax.y < bMin.y || aMin.y > bMax.y));
	//}

	//// returns the result of a collision between Polygons
	//bool Polygon::checkCollisionPolygon(Polygon* polygon1, Polygon* polygon2)
	//{
	//	// do a quick AABB check before doing
	//	// any expensive calculations
	//	if (!checkCollisionAABB(polygon1, polygon2))
	//	{
	//		return false;
	//	}

	//	std::vector<glm::vec2> points1 = polygon1->getLocalPointsInWorldSpace();
	//	std::vector<glm::vec2> points2 = polygon2->getLocalPointsInWorldSpace();

	//	std::vector<glm::vec2> normals1 = polygon1->getLocalNormals();
	//	std::vector<glm::vec2> normals2 = polygon2->getLocalNormals();

	//	// combine all normals
	//	std::vector<glm::vec2> allNormals = normals1;
	//	allNormals.insert(allNormals.end(), normals2.begin(), normals2.end());

	//	float minimumPenetration = FLT_MAX;
	//	glm::vec2 collisionNormal(0);

	//	// for each normal
	//	for (int i = 0; i < allNormals.size(); i++)
	//	{
	//		Projection p1;
	//		Projection p2;

	//		// for each point
	//		for (int j = 0; j < points1.size(); j++)
	//		{
	//			float dot = glm::dot(points1[j], allNormals[i]);

	//			p1.setMin(std::fminf(p1.getMin(), dot));
	//			p1.setMax(std::fmaxf(p1.getMax(), dot));
	//		}
	//		// for each other point
	//		for (int j = 0; j < points2.size(); j++)
	//		{
	//			float dot = glm::dot(points1[j], allNormals[i]);

	//			p2.setMin(std::fminf(p2.getMin(), dot));
	//			p2.setMax(std::fmaxf(p2.getMax(), dot));
	//		}

	//		float dist = p1.getOverlap(p2);

	//		if (!p1.getOverlap(p2))
	//		{
	//			return false;
	//		}

	//		if (dist < minimumPenetration)
	//		{
	//			minimumPenetration = dist;
	//			collisionNormal = allNormals[i];
	//		}
	//	}

	//	aie::Gizmos::add2DLine(glm::vec2(0), glm::normalize(collisionNormal) * 8.0f, glm::vec4(1));

	//	// now to find the contact point
	//	glm::vec2 contactPoint(0);

	//	// one of the collision normals needs to be flipped
	//	// figure out which one
	//	float flipFirst = (std::signbit(glm::dot(polygon1->getPosition(), collisionNormal)) == 1 ? 1.0f : -1.0f);

	//	// get support point(s)
	//	std::vector<glm::vec2> supports1 = getSupports(polygon1, collisionNormal * flipFirst);
	//	std::vector<glm::vec2> supports2 = getSupports(polygon2, collisionNormal * -flipFirst);

	//	if (supports1.size() == 1)
	//	{
	//		contactPoint = supports1[0];
	//	}
	//	else if (supports2.size() == 1)
	//	{
	//		contactPoint = supports2[0];
	//	}
	//	else
	//	{

	//	}

	//	aie::Gizmos::add2DCircle(contactPoint, 3, 8, glm::vec4(1, 1, 0, 1));

	//	polygon1->setVelocity(glm::vec2(0));
	//	polygon1->setAngularVelocity(0);

	//	polygon2->setVelocity(glm::vec2(0));
	//	polygon2->setAngularVelocity(0);

	//	return true;
	//}

	//// check collision between a Polygon and a Sphere
	//bool Polygon::checkCollisionSphere(Polygon* polygon, Sphere* sphere)
	//{
	//	if (polygon->containsPoint(sphere->getPosition()))
	//	{
	//		polygon->setVelocity(glm::vec2(0));
	//		polygon->setAngularVelocity(0);

	//		sphere->setVelocity(glm::vec2(0));
	//		sphere->setAngularVelocity(0);

	//		return true;
	//	}

	//	glm::vec2 point = sphere->getPosition();

	//	std::vector<glm::vec2> points = polygon->getLocalPointsInWorldSpace();

	//	// iterate through points
	//	for (int i = 0; i < points.size(); i++)
	//	{
	//		// treat edge as a line
	//		glm::vec2 lineStart = points[i];
	//		glm::vec2 lineEnd = (i == points.size() - 1 ? points[0] : points[i + 1]);

	//		glm::vec2 line = lineEnd - lineStart;
	//		glm::vec2 toStart = point - lineStart;

	//		float len2 = glm::dot(line, line);

	//		//t is a number in [0,1] describing 
	//		//the closest point on the lineseg as a blend of endpoints.. 
	//		float t = fmaxf(0, fminf(len2, glm::dot(line, toStart))) / len2;

	//		//cp is the position (i.e actual coordinates) of the closest point on the seg
	//		glm::vec2 cp = lineStart + t * line;

	//		if (glm::length(cp - point) <= sphere->getRadius())
	//		{
	//			polygon->setVelocity(glm::vec2(0));
	//			polygon->setAngularVelocity(0);

	//			sphere->setVelocity(glm::vec2(0));
	//			sphere->setAngularVelocity(0);

	//			return true;
	//		}
	//	}

	//	return false;
	//}

	//// check collision between a Polygon and a plane
	//bool Polygon::checkCollisionPlane(Polygon* polygon, Plane* plane)
	//{
	//	glm::vec2 planeNormal = plane->getNormal();

	//	Projection proj;
	//	proj.projectPolygon(polygon, planeNormal);

	//	//aie::Gizmos::add2DLine(minA.point, minA.point - planeNormal * minA.dotAlongNormal, glm::vec4(0, 1, 0, 1));
	//	//aie::Gizmos::add2DLine(maxA.point, maxA.point - planeNormal * maxA.dotAlongNormal, glm::vec4(1, 0, 0, 1));

	//	if (std::signbit(proj.getMin() - plane->getDistance()) == proj.getMax() - plane->getDistance())
	//	{
	//		return false;
	//	}

	//	// now to find the contact point
	//	glm::vec2 contactPoint(0);

	//	// one of the collision normals needs to be flipped
	//	// figure out which one
	//	float flipFirst = (std::signbit(glm::dot(polygon->getPosition(), planeNormal)) == 1 ? 1.0f : -1.0f);

	//	std::vector<glm::vec2> supports = getSupports(polygon, planeNormal * flipFirst);

	//	if (supports.size() == 1)
	//	{
	//		contactPoint = supports[0];
	//	}
	//	else if (supports.size() == 2)
	//	{
	//		contactPoint = 0.5f * (supports[0] + supports[1]);
	//	}

	//	//aie::Gizmos::add2DCircle(contactPoint, 8, 12, glm::vec4(0, 1, 1, 1));

	//	polygon->setPosition(polygon->getPosition() - planeNormal * proj.getMin());

	//	//polygon->setVelocity(glm::vec2(0));
	//	//polygon->setAngularVelocity(0);

	//	return true;
	//}
}
