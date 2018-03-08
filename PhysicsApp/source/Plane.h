#pragma once
#include <glm\ext.hpp>
#include "RigidBody.h"

namespace phy
{
	class Plane : public RigidBody
	{
	public:

		Plane(const glm::vec2 normal = glm::vec2(0, -1), const float distance = 0);
		~Plane() {};

		virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
		virtual void draw();
		virtual void debug() {};

		void setNormal(const glm::vec2 normal) { m_normal = glm::normalize(normal); }
		void setNormal(const float x, const float y) { m_normal = glm::normalize(glm::vec2(x, y)); }
		void setDistance(const float distance) { m_distance = distance; }

		glm::vec2 getPosition() const { return m_distance * m_normal; }
		glm::vec2 getNormal() const { return m_normal; }
		float getDistance() const { return m_distance; }

	protected:
		glm::vec2 m_normal;
		float m_distance;
	};
}
