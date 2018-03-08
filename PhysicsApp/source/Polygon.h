#pragma once
#include <vector>
#include <glm\common.hpp>
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"

namespace phy
{
	class Polygon : public RigidBody
	{
	public:
		Polygon(std::vector<glm::vec2> points);
		Polygon(int numPoints = 4, float radius = 8);

		virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
		virtual void draw();

		std::vector<glm::vec2> getLocalNormals() { return m_localNormals; }
		std::vector<glm::vec2> getLocalPoints() { return m_localPoints; }
		std::vector<glm::vec2> getLocalPointsInWorldSpace();

		glm::vec4 getColor() const { return m_color; }

		void setColor(glm::vec4 color) { m_color = color; }

		bool containsPoint(glm::vec2 point);

		static bool checkCollisionAABB(Polygon* polygon1, Polygon* polygon2);
		static bool checkCollisionPolygon(Polygon* polygon1, Polygon* polygon2);
		static bool checkCollisionSphere(Polygon* polygon, Sphere* sphere);
		static bool checkCollisionPlane(Polygon* polygon, Plane* plane);

	private:

		// is this polygon currently colliding
		bool m_isColliding = false;

		// constant points and normals
		std::vector<glm::vec2> m_points;
		std::vector<glm::vec2> m_normals;

		// local points and normals
		std::vector<glm::vec2> m_localPoints;
		std::vector<glm::vec2> m_localNormals;

		void calculateNormals();

		void calculateLocalPoints();
		void calculateLocalNormals();
	};
}