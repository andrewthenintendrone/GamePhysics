#include "Sphere.h"
#include <Gizmos.h>
#include <glm\ext.hpp>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color) :
RigidBody(ShapeType::SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_color = color;
}

Sphere::~Sphere()
{

}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 36, m_color);
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
