#pragma once
#include <glm\common.hpp>
#include "AABBPoints.h"
#include <limits.h>

enum ShapeTypes
{
	PLANE = 0,
	SPHERE,
	POLYGON,
	SHAPECOUNT
};

namespace phy
{
	

	class RigidBody
	{
	public:
		~RigidBody() {};

		virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
		virtual void draw() = 0;

		virtual AABBPoints getBounds() = 0;
		virtual void calculateMoment() = 0;

		void applyForce(const glm::vec2& force);
		void applyImpulseForce(const glm::vec2& impulseForce);
		void resolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);
		void correctPosition(RigidBody* actor2, float penetration, glm::vec2* collisionNormal = nullptr);

		bool isKinematic() const { return m_isKinematic; }
		glm::vec2 getPosition() const { return m_position; }
		glm::vec2 getVelocity() const { return m_velocity; }
		float getMass() const { return m_isKinematic ? FLT_MAX : m_mass; }
		float invMass() const { return m_isKinematic ? 1.0f / FLT_MAX : 1.0f / m_mass; }
		float getFriction() const { return m_friction; }
		float getElasticity() const { return m_elasticity; }
		float getRotation() const { return m_rotation; }
		float getAngularVelocity() const { return m_angularVelocity; }
		float getMoment() const { return m_moment; }
		virtual ShapeTypes getShapeID() const { return m_shapeID; }
		glm::vec4 getColor() const { return m_color; }

		void setKinematic(const bool b) { m_isKinematic = b; }
		void setPosition(const glm::vec2 position) { m_position = position; }
		void setPosition(const float x, const float y) { m_position = glm::vec2(x, y); }
		void setVelocity(const glm::vec2 velocity) { m_velocity = velocity; }
		void setVelocity(const float x, const float y) { m_velocity = glm::vec2(x, y); }
		void setAngularVelocity(float angularVelocity) { m_angularVelocity = angularVelocity; }
		void setMass(float mass) { m_mass = mass; }
		void setFriction(float friction) { m_friction = friction; }
		void setElasticity(float elasticity) { m_elasticity = elasticity; }
		void setRotation(float rotation) { m_rotation = rotation; }
		void setColor(const glm::vec4 color) { m_color = color; }
		void setColor(const float r, const float g, const float b, const float a) { m_color = glm::vec4(r, g, b, a); }

	protected:

		RigidBody(const ShapeTypes shapeID);

		ShapeTypes m_shapeID;

		bool m_isKinematic = false;

		// linear
		glm::vec2 m_position = glm::vec2(0);
		glm::vec2 m_velocity = glm::vec2(0);
		float m_mass = 1;
		float m_friction = 0.3f;

		// angular
		float m_rotation = 0; // 2D so we only need a single float to represent our rotation
		float m_angularVelocity = 0;
		float m_moment = 0;

		float m_elasticity = 1;
		glm::vec4 m_color = glm::vec4(1);

		const float MIN_LINEAR_THRESHOLD = 0.01f;
		const float MIN_ROTATION_THRESHOLD = 0.1f;
	};
}
