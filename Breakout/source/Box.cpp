#include "Box.h"
#include <Gizmos.h>

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, glm::vec4 color) :
	RigidBody(BOX, position, velocity, 0, mass)
{
	m_width = width;
	m_height = height;
	m_color = color;
}

void Box::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, glm::vec2(m_width, m_height), m_color);
}

bool Box::checkCollision(PhysicsObject* other)
{
	return false;
}
