#pragma once
#include <glm\common.hpp>
//#include "Polygon.h"

// represents a projection of a vec2 along a normal
class Projection
{
public:

	// default constructor
	Projection() {};

	// constructor with min and max
	Projection(float min, float max);

	//void projectPolygon(phy::Polygon* polygon, glm::vec2 axis);

	bool overlaps(const Projection other);
	float getOverlap(const Projection other);
	bool contains(const Projection other);

	float getMin() const { return m_min; }
	float getMax() const { return m_max; }

	void setMin(float min) { m_min = min; }
	void setMax(float max) { m_max = max; }

private:

	float m_min = FLT_MAX;
	float m_max = -FLT_MAX;

	// may be unused
	glm::vec2 m_minV;
	glm::vec2 m_maxV;
};