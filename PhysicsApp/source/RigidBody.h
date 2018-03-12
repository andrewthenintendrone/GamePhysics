#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeTypes shapeID);
	~RigidBody() {};

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	void applyForce(glm::vec2 force, glm::vec2 pos);
	void resolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);
	void correctPosition(RigidBody* actor2, float penetration, glm::vec2* collisionNormal = nullptr);

	bool isKinematic() const { return m_isKinematic; }
	glm::vec2 getPosition() const { return m_position; }
	glm::vec2 getVelocity() const { return m_velocity; }
	float getRotation() const { return m_rotation; }
	float getAngularVelocity() const { return m_angularVelocity; }
	float getMass() const { return m_isKinematic ? INT_MAX : m_mass; }
	float invMass() const { return m_isKinematic ? 1.0f / INT_MAX : 1.0f / m_mass; }
	float getElasticity() const { return m_elasticity; }
	float getMoment() const { return m_moment; }
	float getFriction() const { return m_friction; }
	glm::vec4 getColor() const { return m_color; }

	void setKinematic(bool b) { m_isKinematic = b; }
	void setPosition(const glm::vec2 position) { m_position = position; }
	void setPosition(const float x, const float y) { m_position = glm::vec2(x, y); }
	void setVelocity(const glm::vec2 velocity) { m_velocity = velocity; }
	void setVelocity(const float x, const float y) { m_velocity = glm::vec2(x, y); }
	void setRotation(float rotation) { m_rotation = rotation; }
	void setAngularVelocity(float angularVelocity) { m_angularVelocity = angularVelocity; }
	void setMass(float mass) { m_mass = mass; }
	void setElasticity(float elasticity) { m_elasticity = elasticity; }
	void setFriction(float friction) { m_friction = friction; }
	void setColor(const glm::vec4 color) { m_color = color; }

protected:

	bool m_isKinematic = false;

	// linear
	glm::vec2 m_position = glm::vec2(0);
	glm::vec2 m_velocity = glm::vec2(0);

	// angular
	float m_rotation = 0; // 2D so we only need a single float to represent our rotation
	float m_angularVelocity = 0;

	float m_mass = 1;
	float m_elasticity = 1;
	float m_moment = 0;

	// drag
	float m_friction = 0.3f;
	const float MIN_LINEAR_THRESHOLD = 0.01f;
	const float MIN_ROTATION_THRESHOLD = 0.1f;

	glm::vec4 m_color = glm::vec4(1);
};