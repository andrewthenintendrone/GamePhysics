#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position = glm::vec2(0, 0),
		glm::vec2 velocity = glm::vec2(0, 0), float rotation = 0, float mass = 1);
	~RigidBody() {};

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force, glm::vec2 pos);
	//void applyForceToActor(RigidBody* actor2, glm::vec2 force);
	void resolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2*
	collisionNormal = nullptr);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	glm::vec2 getPosition() { return m_position; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	float getLinearDrag() { return m_linearDrag; }
	float getAngularDrag() { return m_angularDrag; }
	float getElasticity() { return m_elasticity; }
	float getRotation() { return m_rotation; }
	float getAngularVelocity() { return m_angularVelocity; }
	float getmoment() { return m_moment; }

	void setVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void setMass(float mass) { m_mass = mass; }
	void setLinearDrag(float linearDrag) { m_linearDrag = linearDrag; }
	void setAngularDrag(float angularDrag) { m_linearDrag = angularDrag; }
	void setElasticity(float elasticity) { m_elasticity = elasticity; }

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

	const float MIN_LINEAR_THRESHOLD = 0.5f;
	const float MIN_ROTATION_THRESHOLD = 0.05f;
};