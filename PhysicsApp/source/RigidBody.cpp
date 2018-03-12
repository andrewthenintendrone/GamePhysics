#include "RigidBody.h"
#include <glm\ext.hpp>

RigidBody::RigidBody(ShapeTypes shapeID) :
	PhysicsObject(shapeID)
{
	
}

// updates the RigidBody using a fixed timestep
void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKinematic)
	{
		return;
	}

	// remember when applying the force of gravity, mass cancels out
	m_velocity += gravity * timeStep;
	m_position += m_velocity * timeStep;

	m_velocity -= m_velocity * m_friction * timeStep;
	m_rotation += m_angularVelocity * timeStep;
	m_angularVelocity -= m_angularVelocity * m_friction * timeStep;

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0);
	}
	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
	{
		m_angularVelocity = 0;
	}
}

// apply force to the RigidBody at the specified position
void RigidBody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	// Force = mass * acceleration
	// therefore acceleration = Force / mass
	m_velocity += force / m_mass;

	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / (m_moment);
}

// resolves a collision with another RigidBody
void RigidBody::resolveCollision(RigidBody* actor2, glm::vec2 contact,
	glm::vec2* collisionNormal)
{
	// find the vector between their centres, or use the provided direction
	// of force
	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(
	actor2->m_position - m_position);

	// get the vector perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);

	// detemine the total velocity of the contact points for the two objects,
	// for both linear and rotational

	// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);

	// velocity of the contact point on this object
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// velocity of contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2) // they're moving closer
	{
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.
		float mass1 = 1.0f / (invMass() + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (actor2->invMass() + (r2 * r2) / actor2->m_moment);

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		//apply equal and opposite forces
		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->m_position);
	}
}

void RigidBody::correctPosition(RigidBody* actor2, float penetration, glm::vec2* collisionNormal)
{
	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->m_position - m_position);

	const float percent = 0.2f;
	const float slop = 0.01f;

	glm::vec2 correction = fmaxf(penetration - slop, 0.0f) / (invMass() + actor2->invMass()) * percent * normal;

	if (!m_isKinematic)
	{
		m_position -= invMass() * correction;
	}
	if (!actor2->m_isKinematic)
	{
		actor2->m_position += invMass() * correction;
	}
}

