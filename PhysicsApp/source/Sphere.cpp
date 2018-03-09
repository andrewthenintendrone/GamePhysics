#include "Sphere.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <string>
#include "CollisionUtils.h"

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

	// Spheres bounding points are just its radius
	AABBPoints Sphere::getBounds()
	{
		return AABBPoints{ m_position.x - m_radius, m_position.x + m_radius, m_position.y + m_radius, m_position.y - m_radius };
	}
}
