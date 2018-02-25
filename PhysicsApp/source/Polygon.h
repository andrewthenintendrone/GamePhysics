#pragma once
#include <vector>
#include <glm\common.hpp>
#include "RigidBody.h"

// generic Polygon class
namespace abc
{
	class Polygon : public RigidBody
	{
	public:
		Polygon(std::vector<glm::vec2> points);

		virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
		virtual void makeGizmo();

	private:

		std::vector<glm::vec2> m_points;
		std::vector<glm::vec2> m_normals;

		void calculateNormals();

		glm::vec4 m_color = glm::vec4(0.5f, 0, 1, 1);

		int m_numFaces = 0;
	};
}
