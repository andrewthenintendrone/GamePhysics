#include "RigidBody.h"

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) : PhysicsObject(shapeID)
{
	m_mass = mass;
	m_velocity = velocity;
	m_position = position;
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	applyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
}

void RigidBody::debug()
{

}

void RigidBody::applyForce(glm::vec2 force)
{
	m_velocity += glm::vec2(force / m_mass);
}

void RigidBody::applyForceToActor(RigidBody* actor2, glm::vec2 force)
{
	// first apply the input force to the input actor
	actor2->applyForce(force);

	// then apply equal and opposite force to this actor
	this->applyForce(-force);
}

