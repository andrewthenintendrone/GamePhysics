#include "RigidBody.h"
#include <glm\ext.hpp>

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) : PhysicsObject(shapeID)
{
	m_mass = mass;
	m_velocity = velocity;
	m_position = position;
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
	{
		m_angularVelocity = 0;
	}

	m_position += m_velocity * timeStep;
	applyForce(gravity * m_mass * timeStep);
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

void RigidBody::resolveCollision(RigidBody* actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f
		;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
		glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())));

	glm::vec2 force = normal * j;
	applyForceToActor(actor2, force);
}
