#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeTypes shapeID, glm::vec2 position = glm::vec2(0),
		glm::vec2 velocity = glm::vec2(0), float rotation = 0, float mass = 1);
	~RigidBody() {};

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force);
	void resolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);
	void correctPosition(RigidBody* actor2, float penetration, glm::vec2* collisionNormal = nullptr);

	bool isKinematic() const { return m_isKinematic; }
	glm::vec2 getPosition() const { return m_position; }
	glm::vec2 getVelocity() const { return m_velocity; }
	float getMass() const { return m_isKinematic ? INT_MAX : m_mass; }
	float invMass() const { return m_isKinematic ? 1.0f / INT_MAX : 1.0f / m_mass; }
	float getLinearDrag() const { return m_linearDrag; }
	float getAngularDrag() const { return m_angularDrag; }
	float getElasticity() const { return m_elasticity; }
	float getRotation() const { return m_rotation; }
	float getAngularVelocity() const { return m_angularVelocity; }
	float getMoment() const { return m_moment; }

	void setKinematic(bool b) { m_isKinematic = b; }
	void setPosition(glm::vec2 position) { m_position = position; }
	void setVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void setAngularVelocity(float angularVelocity) { m_angularVelocity = angularVelocity; }
	void setMass(float mass) { m_mass = mass; }
	void setLinearDrag(float linearDrag) { m_linearDrag = linearDrag; }
	void setAngularDrag(float angularDrag) { m_angularDrag = angularDrag; }
	void setElasticity(float elasticity) { m_elasticity = elasticity; }
	void setRotation(float rotation) { m_rotation = rotation; }

protected:
	bool m_isKinematic = false;

	// linear
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_linearDrag = 0.3f;
	float m_angularDrag = 0.3f;
	float m_elasticity = 1;

	// angular
	float m_rotation = 0; // 2D so we only need a single float to represent our rotation
	float m_angularVelocity = 0;
	float m_moment = 0;

	const float MIN_LINEAR_THRESHOLD = 0.01f;
	const float MIN_ROTATION_THRESHOLD = 0.1f;
};