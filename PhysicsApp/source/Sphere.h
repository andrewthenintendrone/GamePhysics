#pragma once
#include "RigidBody.h"

namespace phy
{
	class Sphere : public RigidBody
	{
	public:
		Sphere(const float radius = 1);

		~Sphere() {};
		virtual void draw();

		virtual AABBPoints getBounds();

		void setRadius(const float radius) { m_radius = radius; }
		float getRadius() const { return m_radius; }

	protected:

		float m_radius;
	};
}
