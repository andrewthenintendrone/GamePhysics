#include "PhysicsScene.h"
#include <algorithm>
#include <list>
#include "RigidBody.h"
#include <iostream>
#include "Circle.h"
#include "Plane.h"
#include "Box.h"
#include "Aabb.h"
#include <glm\ext.hpp>

// function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane, PhysicsScene::plane2Circle, PhysicsScene::plane2AABB,
	PhysicsScene::circle2Plane, PhysicsScene::circle2Circle, PhysicsScene::circle2AABB,
	PhysicsScene::AABB2Plane, PhysicsScene::AABB2Circle, PhysicsScene::AABB2AABB
};

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
	
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	m_actors.erase(std::remove(m_actors.begin(), m_actors.end(), actor), m_actors.end());
}

void PhysicsScene::update(float dt)
{
	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			RigidBody* pRigid = dynamic_cast<RigidBody*>(pActor);
			if (pRigid != nullptr)
			{
				pActor->fixedUpdate(m_gravity, m_timeStep);
			}
		}

		accumulatedTime -= m_timeStep;

		checkForCollison();
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::checkForCollison()
{
	int actorCount = (int)m_actors.size();

	// need to check for collisions against all objects except this one
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			// using function pointers
			int functionIdx = (shapeId1 * ShapeTypes::SHAPECOUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did the collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}


// ---------------------------------------------------------
// colision detection functions
// ---------------------------------------------------------

// test collision between 2 planes
bool PhysicsScene::plane2Plane(PhysicsObject* a, PhysicsObject* b)
{
	return false;
}

// test collision between a plane and a circle
bool PhysicsScene::plane2Circle(PhysicsObject* a, PhysicsObject* b)
{
	return circle2Plane(b, a);
}

// test collision between a plane and a box
bool PhysicsScene::plane2Box(PhysicsObject* a, PhysicsObject* b)
{
	return box2Plane(b, a);
}

// test collision between a plane and an axis aligned bounding box
bool PhysicsScene::plane2AABB(PhysicsObject* a, PhysicsObject* b)
{
	return AABB2Plane(b, a);
}

// text collision between a circle and a plane
bool PhysicsScene::circle2Plane(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to circle and plane
	Circle* circle = dynamic_cast<Circle*>(a);
	Plane* plane = dynamic_cast<Plane*>(b);

	// if we are successful then check for collision
	if (circle != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();

		float collisionDistance = glm::dot(circle->getPosition(), collisionNormal) - plane->getDistance();

		// we have collidedwith the back side of the plane
		if (collisionDistance < 0)
		{
			collisionDistance *= -1;
		}

		if (collisionDistance <= circle->getRadius())
		{
			circle->setVelocity(glm::vec2(0));
			return true;
		}
	}
	return false;
}

// test collision between 2 circles
bool PhysicsScene::circle2Circle(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to circle and circle
	Circle* circle1 = dynamic_cast<Circle*>(a);
	Circle* circle2 = dynamic_cast<Circle*>(b);

	// if we are successful then test for collision
	if (circle1 != nullptr && circle2 != nullptr)
	{
		// find the offset of the two circles
		glm::vec2 offset = circle1->getPosition() - circle2->getPosition();

		// calculate the squared distance (avoids square root)
		float squaredDistance = (offset.x * offset.x + offset.y * offset.y);

		// combine the radii and square the result
		float r = circle1->getRadius() + circle2->getRadius();
		r *= r;

		// if the squared distance is less than the squared radius
		if (squaredDistance < r)
		{
			circle1->setVelocity(glm::vec2(0));
			circle2->setVelocity(glm::vec2(0));
			return true;
		}
	}

	return false;
}

// test collision between a circle and a box
bool PhysicsScene::circle2Box(PhysicsObject* a, PhysicsObject* b)
{
	return box2Circle(b, a);
}

// test collision between a circle and an axis aligned bounding box
bool PhysicsScene::circle2AABB(PhysicsObject* a, PhysicsObject* b)
{
	return AABB2Circle(b, a);
}

// test collision between a box and a plane
bool PhysicsScene::box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box = dynamic_cast<Box*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	//if we are successful then test for collision
	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		float radius = 0.5f * std::fminf(box->getWidth(), box->getHeight());
		float penetration = 0;

		// which side is the center of mass on?
		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		float comFromPlane = glm::dot(box->getPosition() - planeOrigin,
			plane->getNormal());

		// check all four corners to see if we've hit the plane
		for (float x = -box->getExtents().x; x < box->getWidth(); x += box->getWidth())
		{
			for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight())
			{
				// get the position of the corner in world space
				glm::vec2 p = box->getPosition() + x * box->getLocalX() +
					y * box->getLocalY();

				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());

				// this is the total velocity of the point
				float velocityIntoPlane = glm::dot(box->getVelocity() + box->getAngularVelocity() *
					(-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());

				// if this corner is on the opposite side from the COM,
				// and moving further in, we need to resolve the collision
				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane >= 0) ||
					(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane <= 0))
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;

					if (comFromPlane >= 0)
					{
						if (penetration > distFromPlane)
						{
							penetration = distFromPlane;
						}
					}
					else
					{
						if (penetration < distFromPlane)
						{
							penetration = distFromPlane;
						}
					}
				}
			}
		}
		// we've had a hit - typically only two corners can contact
		if (numContacts > 0)
		{
			// get the average collision velocity into the plane
			// (covers linear and rotational velocity of all corners involved)
			float collisionV = contactV / (float)numContacts;

			// get the acceleration required to stop (restitution = 0) or reverse
			// (restitution = 1) the average velocity into the plane
			glm::vec2 acceleration = -plane->getNormal() *
				((1.0f + box->getElasticity()) * collisionV);

			// and the average position at which we'll apply the force
			// (corner or edge centre)
			glm::vec2 localContact = (contact / (float)numContacts) - box->getPosition();

			// this is the perpendicular distance we apply the force at relative to
			// the COM, so Torque = F*r
			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y,
				-plane->getNormal().x));

			// work out the "effective mass" - this is a combination of moment of
			// inertia and mass, and tells us how much the contact point velocity
			// will change with the force we're applying
			float mass0 = 1.0f / (1.0f / box->getMass() + (r * r) / box->getMoment());

			// and apply the force
			box->applyForce(acceleration * mass0, localContact);

			box->setPosition(box->getPosition() - plane->getNormal() * penetration);
		}
	}
	return false;
}

// test collision between a box and a circle
bool PhysicsScene::box2Circle(PhysicsObject* a, PhysicsObject* b)
{
	Box* box = dynamic_cast<Box*>(a);
	Circle* circle = dynamic_cast<Circle*>(b);

	if (box != nullptr && circle != nullptr)
	{
		glm::vec2 circlePos = circle->getPosition() - box->getPosition();
		float w2 = box->getWidth() / 2, h2 = box->getHeight() / 2;

		int numContacts = 0;

		glm::vec2 contact(0, 0); // contact is in our box coordinates

		// check the four corners to see if any of them are inside the circle
		for (float x = -w2; x <= w2; x += box->getWidth())
		{
			for (float y = -h2; y <= h2; y += box->getHeight())
			{
				glm::vec2 p = x * box->getLocalX() + y * box->getLocalY();
				glm::vec2 dp = p - circlePos;
				if (dp.x * dp.x + dp.y * dp.y < circle->getRadius() * circle->getRadius())
				{
					numContacts++;
					contact += glm::vec2(x, y);
				}
			}
		}
		glm::vec2* direction = nullptr;
		// get the local position of the circle centre
		glm::vec2 localPos(glm::dot(box->getLocalX(), circlePos),
			glm::dot(box->getLocalY(), circlePos));
		if (localPos.y < h2 && localPos.y > -h2)
		{
			if (localPos.x > 0 && localPos.x < w2 + circle->getRadius())
			{
				numContacts++;
				contact += glm::vec2(w2, localPos.y);
				direction = new glm::vec2(box->getLocalX());
			}
			if (localPos.x < 0 && localPos.x > -(w2 + circle->getRadius()))
			{
				numContacts++;
				contact += glm::vec2(-w2, localPos.y);
				direction = new glm::vec2(-box->getLocalX());
			}
		}
		if (localPos.x < w2 && localPos.x > -w2)
		{
			if (localPos.y > 0 && localPos.y < h2 + circle->getRadius())
			{
				numContacts++;
				contact += glm::vec2(localPos.x, h2);
				direction = new glm::vec2(box->getLocalY());
			}
			if (localPos.y < 0 && localPos.y > -(h2 + circle->getRadius()))
			{
				numContacts++;
				contact += glm::vec2(localPos.x, -h2);
				direction = new glm::vec2(-box->getLocalY());
			}
		}
		if (numContacts > 0)
		{
			// average, and convert back into world coords
			contact = box->getPosition() + (1.0f / numContacts) *
				(box->getLocalX() * contact.x + box->getLocalY() * contact.y);
			box->resolveCollision(circle, contact, direction);
		}
		delete direction;
	}
	return false;
}

// test collision between 2 boxes
bool PhysicsScene::box2Box(PhysicsObject* a, PhysicsObject* b)
{
	Box* box1 = dynamic_cast<Box*>(a);
	Box* box2 = dynamic_cast<Box*>(b);

	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 boxPos = box2->getCenter() - box1->getCenter();
		glm::vec2 normal;
		glm::vec2 contactForce1, contactForce2;
		glm::vec2 contact(0, 0);
		int numContacts = 0;

		box1->checkBoxCorners(*box2, contact, numContacts, normal, contactForce1);
		if (box2->checkBoxCorners(*box1, contact, numContacts,
			normal, contactForce2))
		{
			normal = -normal;
		}
		if (numContacts > 0)
		{
			glm::vec2 contactForce = 0.5f * (contactForce1 - contactForce2);
			box1->setPosition(box1->getPosition() - contactForce);
			box2->setPosition(box2->getPosition() + contactForce);
			box1->resolveCollision(box2, contact / float(numContacts), &normal);
			return true;
		}
	}
	return false;
}

// test collision between a box and an axis aligned bounding box
bool PhysicsScene::box2AABB(PhysicsObject* a, PhysicsObject* b)
{
	return false;
}

// test collision between an axis aligned bounding box and a plane
bool PhysicsScene::AABB2Plane(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to Aabb and Plane
	Aabb* aabb = dynamic_cast<Aabb*>(a);
	Plane* plane = dynamic_cast<Plane*>(b);

	// if we are successful then test for collision
	if (aabb != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();

		float cornerOffsets[4];
		bool signs[4];

		for (int i = 0; i < 4; i++)
		{
			cornerOffsets[i] = glm::dot(aabb->getCorner(i + 1), collisionNormal) - plane->getDistance();
			signs[i] = std::signbit(cornerOffsets[i]);
		}

		if (signs[0] != signs[3] || signs[1] != signs[2])
		{
			aabb->setVelocity(glm::vec2(0));
			return true;
		}
	}
	return false;
}

// test collision between an axis aligned bounding box and a circle
bool PhysicsScene::AABB2Circle(PhysicsObject* a, PhysicsObject* b)
{
	// try to cast objects to Aabb and circle
	Aabb* aabb = dynamic_cast<Aabb*>(a);
	Circle* circle = dynamic_cast<Circle*>(b);

	// if we are successful then test for collision
	if (aabb != nullptr && circle != nullptr)
	{
		glm::vec2 collisionPoint = circle->getPosition() + glm::normalize(aabb->getPosition() - circle->getPosition()) * circle->getRadius();

		if (aabb->containsPoint(collisionPoint))
		{
			aabb->setVelocity(glm::vec2(0));
			circle->setVelocity(glm::vec2(0));
			return true;
		}
	}

	return false;
}

// test collision between an axis aligned bounding box and a box
bool PhysicsScene::AABB2Box(PhysicsObject* a, PhysicsObject* b)
{
	return false;
}

// test collision between 2 axis aligned bounding boxes
bool PhysicsScene::AABB2AABB(PhysicsObject* a, PhysicsObject* b)
{
	Aabb* Aabb1 = dynamic_cast<Aabb*>(a);
	Aabb* Aabb2 = dynamic_cast<Aabb*>(b);

	if (Aabb1 != nullptr && Aabb2 != nullptr)
	{
		// get min / max for both Aabbs
		glm::vec2 aMin = Aabb1->getMin();
		glm::vec2 aMax = Aabb1->getMax();
		glm::vec2 bMin = Aabb2->getMin();
		glm::vec2 bMax = Aabb2->getMax();

		// test for intersection
		if (aMin.x <= bMax.x && aMax.x >= bMin.x && aMin.y <= bMax.y && aMax.y >= bMin.y)
		{
			// collision
			Aabb1->setVelocity(glm::vec2(0));
			Aabb2->setVelocity(glm::vec2(0));

			return true;
		}
	}

	return false;
}