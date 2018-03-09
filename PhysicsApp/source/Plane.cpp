#include "Plane.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
#include "CollisionUtils.h"

namespace phy
{
	Plane::Plane(const glm::vec2 normal, float distance) :
		RigidBody(ShapeTypes::PLANE)
	{
		m_normal = glm::normalize(normal);
		m_distance = distance;
		m_isKinematic = true;
	}

	void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
	{

	}

	void Plane::draw()
	{
		float lineSegmentLength = 3000;
		glm::vec2 centerPoint = m_normal * m_distance;
		// easy to rotate normal through 90 degrees around z
		glm::vec2 parallel(m_normal.y, -m_normal.x);
		glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
		glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
		aie::Gizmos::add2DLine(start, end, m_color);
	}

	// an infinite plane has infinite bounding points
	AABBPoints Plane::getBounds()
	{
		return AABBPoints{ -FLT_MAX, FLT_MAX, FLT_MAX, -FLT_MAX };
	}
}
