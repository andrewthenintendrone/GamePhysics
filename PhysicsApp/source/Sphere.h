#pragma once
#include "RigidBody.h"
#include <glm\vec4.hpp>

#include "Renderer2D.h"

class Sphere : public RigidBody
{
public:
	Sphere();

	~Sphere();
	virtual void draw();

	float getRadius() const { return m_radius; }

	void setRadius(const float radius) { m_radius = radius; }

	void calculateMoment();

protected:

	float m_radius;
};