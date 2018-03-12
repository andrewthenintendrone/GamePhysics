#include "Sphere.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <string>
#include <math.h>

Sphere::Sphere() :
RigidBody(ShapeTypes::SPHERE)
{
	
}

Sphere::~Sphere()
{

}

void Sphere::draw()
{
	//glm::vec2 end = glm::vec2(std::cosf(m_rotation), std::sinf(m_rotation)) *
		//m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_color);

	glm::vec4 invColor(1.0f - m_color.r, 1.0f - m_color.g, 1.0f - m_color.b, 1.0f);

	float angles[4];
	angles[0] = -15;
	angles[1] = -115;
	angles[2] = -165;
	angles[3] = 115;

	glm::vec2 points[4];

	for (int i = 0; i < 4; i++)
	{
		float theta = glm::radians(angles[i] + m_rotation + 90);

		float sn = std::sinf(theta);
		float cs = std::cosf(theta);

		glm::vec2 point(sn, -cs);

		point *= m_radius;
		point += m_position;

		points[i] = glm::vec2(point);
	}

	aie::Gizmos::add2DLine(points[0], points[2], invColor);
	aie::Gizmos::add2DLine(points[1], points[3], invColor);


	//aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1));
}

// calculate moment of inertia
void Sphere::calculateMoment()
{
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}
