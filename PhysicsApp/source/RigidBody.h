#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position,
		glm::vec2 velocity, float rotation, float mass);
	~RigidBody() {};

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force);
	void applyForceToActor(RigidBody* actor2, glm::vec2 force);
	void resolveCollision(RigidBody* actor2);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	glm::vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	float getLinearDrag() { return m_linearDrag; }
	float getAngularDrag() { return m_angularDrag; }

	void setVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void setMass(float mass) { m_mass = mass; }
	void setLinearDrag(float linearDrag) { m_linearDrag = linearDrag; }
	void setAngularDrag(float angularDrag) { m_linearDrag = angularDrag; }

protected:
	glm::vec2 m_position;
	float m_rotation; // 2D so we only need a single float to represent our rotation
	glm::vec2 m_velocity;
	float m_angularVelocity;
	float m_mass;
	float m_linearDrag;
	float m_angularDrag;
	float m_elasticity;

	const float MIN_LINEAR_THRESHOLD = 0.01f;
	const float MIN_ROTATION_THRESHOLD = 0.1f;
};