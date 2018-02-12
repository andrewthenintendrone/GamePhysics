#include "Plane.h"
#include <Gizmos.h>
#include <glm\ext.hpp>

Plane::Plane() :
	PhysicsObject(ShapeType::PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec2(0, 1);
}

Plane::Plane(glm::vec2 normal, float distance) :
	PhysicsObject(ShapeType::PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
}

void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Plane::makeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	// easy to rotate normal through 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 color(1, 1, 1, 1);
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, color);
}

void Plane::resetPosition()
{

}

void Plane::resolveCollision(RigidBody* actor2)
{
	//glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	//glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float elasticity = 1;
	float j = glm::dot(actor2->getVelocity() - (1 + elasticity) * actor2->getVelocity(), m_normal);

	glm::vec2 force = m_normal * j;
	actor2->setVelocity(force);
}