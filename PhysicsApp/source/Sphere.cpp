#include "Sphere.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <string>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color) :
RigidBody(ShapeTypes::SPHERE, position, velocity, 0, mass)
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
	glm::vec2 end = glm::vec2(std::cosf(m_rotation), std::sinf(m_rotation)) *
		m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_color);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1));
}

