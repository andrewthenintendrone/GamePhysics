#include "Plane.h"
#include <Gizmos.h>
#include <glm\ext.hpp>

Plane::Plane() :
	RigidBody(ShapeTypes::PLANE)
{
	// planes are always kinematic
	m_isKinematic = true;
}

void Plane::draw()
{
	float lineSegmentLength = 3000;
	glm::vec2 centerPoint = m_normal * m_distance;
	// easy to rotate normal through 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 color(1, 1, 1, 1);
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, color);
}

void Plane::resolveCollision(RigidBody* actor2, const glm::vec2 contact)
{
	// the plane isn't moving, so the relative velocity is just actor2's velocity
	glm::vec2 vRel = actor2->getVelocity();
	float e = actor2->getElasticity();
	float j = glm::dot(-(1 + e) * (vRel), m_normal) * actor2->getMass();
	glm::vec2 force = m_normal * j;
	actor2->applyForce(force, contact - actor2->getPosition());
}

