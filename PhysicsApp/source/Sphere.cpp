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
		calculateMoment();
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
		float left = m_position.x - m_radius;
		float right = m_position.x + m_radius;
		float top = m_position.y + m_radius;
		float bottom = m_position.y - m_radius;

		return AABBPoints{ left, right, top, bottom };
	}

	// calculate moment of inertia
	void Sphere::calculateMoment()
	{
		// moment of inertia for a sphere is 2/5 mr^2
		m_moment = 0.4f * m_mass * (m_radius * m_radius);
	}
}
