#pragma once
#include "RigidBody.h"

<<<<<<< HEAD:PhysicsApp/source/Sphere.h
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
=======
#include "Renderer2D.h"

class Circle : public RigidBody
{
public:
	Circle(glm::vec2 position = glm::vec2(0, 0), glm::vec2 velocity = glm::vec2(0, 0),
		float mass = 1, float radius = 1, glm::vec4 color = glm::vec4(1));

	~Circle();
	virtual void makeGizmo();
>>>>>>> parent of 89a0f71... done changing circle back to sphere:PhysicsApp/source/Circle.h

		virtual void calculateMoment();

		float m_radius;
	};
}
