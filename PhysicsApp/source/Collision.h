#pragma once
#include <glm\glm.hpp>

namespace phy
{
	class RigidBody;

	// contains info about a Collision
	class Collision
	{
		RigidBody* object1;
		RigidBody* object2;

		float penetration;
		glm::vec2 normal;
	};
}

