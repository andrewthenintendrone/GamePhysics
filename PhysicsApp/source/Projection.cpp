#include "Projection.h"
#include "Polygon.h"
#include <glm\ext.hpp>

Projection::Projection(float min, float max)
{
	m_min = min;
	m_max = max;
}

// create projection from projecting a polygon along an axis
void Projection::projectPolygon(phy::Polygon* polygon, glm::vec2 axis)
{
	std::vector<glm::vec2> points = polygon->getLocalPointsInWorldSpace();

	for (std::vector<glm::vec2>::iterator iter = points.begin(); iter != points.end(); iter++)
	{
		float dot = glm::dot(*iter, axis);

		m_min = std::fminf(m_min, dot);
		m_max = std::fmaxf(m_max, dot);
	}
}

// returns true if this projection overlaps the other
bool Projection::overlaps(const Projection other)
{
	return getOverlap(other) >= 0;
}

float Projection::getOverlap(const Projection other)
{
	return std::fminf(m_max, other.m_max) - std::fmaxf(m_min, other.m_min);
}

bool Projection::contains(const Projection other)
{
	return (m_min < other.m_min && m_max > other.m_max);
}
