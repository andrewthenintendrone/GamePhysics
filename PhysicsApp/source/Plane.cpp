#include "Plane.h"
#include <Gizmos.h>
#include <glm\ext.hpp>

Plane::Plane() :
	RigidBody(ShapeTypes::PLANE, glm::vec2(0), glm::vec2(0), 0, 1)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec2(0, 1);
	m_isKinematic = true;
}

Plane::Plane(glm::vec2 normal, float distance) :
	RigidBody(ShapeTypes::PLANE, glm::vec2(0), glm::vec2(0), 0, 1)
{
	m_normal = glm::normalize(normal);
	m_distanceToOrigin = distance;
	m_isKinematic = true;
}

void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Plane::makeGizmo()
{
	float lineSegmentLength = 3000;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	// easy to rotate normal through 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 color(1, 1, 1, 1);
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, color);
}
