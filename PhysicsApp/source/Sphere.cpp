#include "Sphere.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <string>

namespace phy
{
	Sphere::Sphere(const float radius) :
		RigidBody(ShapeTypes::SPHERE)
	{
		m_radius = radius;
	}

	void Sphere::draw()
	{
		aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_color);

		glm::vec2 end = glm::vec2(std::cosf(m_rotation), std::sinf(m_rotation)) *
			m_radius;

		aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1));
	}
}
