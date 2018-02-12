#include "Sphere.h"
#include <Gizmos.h>
#include <glm\ext.hpp>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color) :
RigidBody(ShapeType::SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_color = color;
	m_moment = 0.5f * mass * radius * radius;
}

Sphere::~Sphere()
{

}

void Sphere::makeGizmo()
{
	glm::vec2 end = glm::vec2(cos(m_rotation), sin(m_rotation)) *
		m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 36, m_color);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1));
}

bool Sphere::checkCollision(PhysicsObject* pOther)
{
	// pOther is a sphere
	/*Sphere* pSphere = dynamic_cast<Sphere*>(pOther);

	if (glm::distance(m_position, pSphere->getPosition()) <= (m_radius + pSphere->getRadius()))
	{
		return true;
	}*/

	return false;
}
