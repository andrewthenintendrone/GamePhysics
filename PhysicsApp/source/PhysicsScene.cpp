#include "PhysicsScene.h"
#include <algorithm>
#include <list>
#include "RigidBody.h"
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Aabb.h"
#include "Polygon.h"
#include <glm\ext.hpp>

// function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

//static fn collisionFunctionArray[] =
//{
//	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2AABB, PhysicsScene::plane2Box,
//	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2AABB, PhysicsScene::sphere2Box,
//	PhysicsScene::AABB2Plane, PhysicsScene::AABB2Sphere, PhysicsScene::AABB2AABB, PhysicsScene::AABB2Box,
//	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2AABB, PhysicsScene::box2Box
//};

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
			PhysicsObject* pRigid = dynamic_cast<PhysicsObject*>(pActor);
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
			phy::Polygon* object1 = reinterpret_cast<phy::Polygon*>(m_actors[outer]);
			phy::Polygon* object2 = reinterpret_cast<phy::Polygon*>(m_actors[inner]);
			//int shapeId1 = object1->getShapeID();
			//int shapeId2 = object2->getShapeID();

			// do polygon SAT
			if (object1 != nullptr && object2 != nullptr)
			{
				phy::Polygon::checkCollision(object1, object2);
			}

			// using function pointers
			//int functionIdx = (shapeId1 * ShapeTypes::SHAPECOUNT) + shapeId2;
			//fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			//if (collisionFunctionPtr != nullptr)
			//{
			//	// did the collision occur?
			//	collisionFunctionPtr(object1, object2);
			//}
		}
	}
}


// ---------------------------------------------------------
// colision detection functions
// ---------------------------------------------------------

// test collision between 2 planes
//bool PhysicsScene::plane2Plane(PhysicsObject* a, PhysicsObject* b)
//{
//	return false;
//}
//
//// test collision between a plane and a sphere
//bool PhysicsScene::plane2Sphere(PhysicsObject* a, PhysicsObject* b)
//{
//	return sphere2Plane(b, a);
//}
//
//// test collision between a plane and a box
//bool PhysicsScene::plane2Box(PhysicsObject* a, PhysicsObject* b)
//{
//	return box2Plane(b, a);
//}
//
//// test collision between a plane and an axis aligned bounding box
//bool PhysicsScene::plane2AABB(PhysicsObject* a, PhysicsObject* b)
//{
//	return AABB2Plane(b, a);
//}
//
//// text collision between a sphere and a plane
//bool PhysicsScene::sphere2Plane(PhysicsObject* a, PhysicsObject* b)
//{
//	Sphere* sphere = dynamic_cast<Sphere*>(a);
//	Plane* plane = dynamic_cast<Plane*>(b);
//
//	//if we are successful then test for collision
//	if (sphere != nullptr && plane != nullptr)
//	{
//		glm::vec2 collisionNormal = plane->getNormal();
//		float sphereToPlane = glm::dot(
//			sphere->getPosition(),
//			plane->getNormal()) - plane->getDistance();
//
//		// if we are behind plane then we flip the normal
//		if (sphereToPlane < 0)
//		{
//			collisionNormal *= -1;
//			sphereToPlane *= -1;
//		}
//
//		float intersection = sphere->getRadius() - sphereToPlane;
//
//		if (intersection > 0)
//		{
//			glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
//
//			// contact force
//			sphere->setPosition(sphere->getPosition() +
//				collisionNormal * (sphere->getRadius() - sphereToPlane));
//
//			plane->resolveCollision(sphere, contact);
//			return true;
//		}
//	}
//	return false;
//}
//
//// test collision between 2 spheres
//bool PhysicsScene::sphere2Sphere(PhysicsObject* a, PhysicsObject* b)
//{
//	Sphere* sphere1 = dynamic_cast<Sphere*>(a);
//	Sphere* sphere2 = dynamic_cast<Sphere*>(b);
//
//	if (sphere1 != nullptr && sphere2 != nullptr)
//	{
//		glm::vec2 delta = sphere2->getPosition() - sphere1->getPosition();
//		float distance = glm::length(delta);
//		float intersection = sphere1->getRadius() + sphere2->getRadius() - distance;
//
//		if (intersection > 0)
//		{
//			glm::vec2 contactForce = 0.5f * (distance - (sphere1->getRadius() +
//				sphere2->getRadius())) * delta / distance;
//
//			//sphere1->setPosition(sphere1->getPosition() + contactForce);
//			//sphere2->setPosition(sphere2->getPosition() - contactForce);
//
//			// respond to the collision
//			sphere1->resolveCollision(sphere2, 0.5f * (sphere1->getPosition() +
//				sphere2->getPosition()));
//			return true;
//		}
//	}
//
//
//	return false;
//}
//
//// test collision between a sphere and a box
//bool PhysicsScene::sphere2Box(PhysicsObject* a, PhysicsObject* b)
//{
//	return box2Sphere(b, a);
//}
//
//// test collision between a sphere and an axis aligned bounding box
//bool PhysicsScene::sphere2AABB(PhysicsObject* a, PhysicsObject* b)
//{
//	return AABB2Sphere(b, a);
//}
//
//// test collision between a box and a plane
//bool PhysicsScene::box2Plane(PhysicsObject* a, PhysicsObject* b)
//{
//	Box* box = dynamic_cast<Box*>(a);
//	Plane* plane = dynamic_cast<Plane*>(b);
//
//	//if we are successful then test for collision
//	if (box != nullptr && plane != nullptr)
//	{
//		int numContacts = 0;
//		glm::vec2 contact(0, 0);
//		float contactV = 0;
//		float radius = 0.5f * std::fminf(box->getWidth(), box->getHeight());
//		float penetration = 0;
//
//		// which side is the centre of mass on?
//		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
//		float comFromPlane = glm::dot(box->getPosition() - planeOrigin,
//			plane->getNormal());
//
//		// check all four corners to see if we've hit the plane
//		for (float x = -box->getExtents().x; x < box->getWidth(); x += box->getWidth())
//		{
//			for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight())
//			{
//				// get the position of the corner in world space
//				glm::vec2 p = box->getPosition() + x * box->getLocalX() +
//					y * box->getLocalY();
//
//				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());
//
//				// this is the total velocity of the point
//				float velocityIntoPlane = glm::dot(box->getVelocity() + box->getAngularVelocity() *
//					(-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());
//
//				// if this corner is on the opposite side from the COM,
//				// and moving further in, we need to resolve the collision
//				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane >= 0) ||
//					(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane <= 0))
//				{
//					numContacts++;
//					contact += p;
//					contactV += velocityIntoPlane;
//
//					if (comFromPlane >= 0)
//					{
//						if (penetration > distFromPlane)
//						{
//							penetration = distFromPlane;
//						}
//					}
//					else
//					{
//						if (penetration < distFromPlane)
//						{
//							penetration = distFromPlane;
//						}
//					}
//				}
//			}
//		}
//		// we've had a hit - typically only two corners can contact
//		if (numContacts > 0)
//		{
//			// get the average collision velocity into the plane
//			// (covers linear and rotational velocity of all corners involved)
//			float collisionV = contactV / (float)numContacts;
//
//			// get the acceleration required to stop (restitution = 0) or reverse
//			// (restitution = 1) the average velocity into the plane
//			glm::vec2 acceleration = -plane->getNormal() *
//				((1.0f + box->getElasticity()) * collisionV);
//
//			// and the average position at which we'll apply the force
//			// (corner or edge centre)
//			glm::vec2 localContact = (contact / (float)numContacts) - box->getPosition();
//
//			// this is the perpendicular distance we apply the force at relative to
//			// the COM, so Torque = F*r
//			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y,
//				-plane->getNormal().x));
//
//			// work out the "effective mass" - this is a combination of moment of
//			// inertia and mass, and tells us how much the contact point velocity
//			// will change with the force we're applying
//			float mass0 = 1.0f / (box->invMass() + (r * r) / box->getMoment());
//
//			// and apply the force
//			box->applyForce(acceleration * mass0, localContact);
//
//			box->setPosition(box->getPosition() - plane->getNormal() * penetration);
//		}
//	}
//	return false;
//}
//
//// test collision between a box and a sphere
//bool PhysicsScene::box2Sphere(PhysicsObject* a, PhysicsObject* b)
//{
//	Box* box = dynamic_cast<Box*>(a);
//	Sphere* sphere = dynamic_cast<Sphere*>(b);
//
//	if (box != nullptr && sphere != nullptr)
//	{
//		glm::vec2 circlePos = sphere->getPosition() - box->getPosition();
//		float w2 = box->getWidth() / 2, h2 = box->getHeight() / 2;
//		int numContacts = 0;
//		glm::vec2 contact(0, 0); // contact is in our box coordinates
//
//		// check the four corners to see if any of them are inside the circle
//		for (float x = -w2; x <= w2; x += box->getWidth())
//		{
//			for (float y = -h2; y <= h2; y += box->getHeight())
//			{
//				glm::vec2 p = x * box->getLocalX() + y * box->getLocalY();
//				glm::vec2 dp = p - circlePos;
//				if (dp.x * dp.x + dp.y * dp.y < sphere->getRadius() * sphere->getRadius())
//				{
//					numContacts++;
//					contact += glm::vec2(x, y);
//				}
//			}
//		}
//		glm::vec2* direction = nullptr;
//
//		// get the local position of the circle centre
//		glm::vec2 localPos(glm::dot(box->getLocalX(), circlePos),
//			glm::dot(box->getLocalY(), circlePos));
//
//		if (localPos.y < h2 && localPos.y > -h2)
//		{
//			if (localPos.x > 0 && localPos.x < w2 + sphere->getRadius())
//			{
//				numContacts++;
//				contact += glm::vec2(w2, localPos.y);
//				direction = new glm::vec2(box->getLocalX());
//			}
//			if (localPos.x < 0 && localPos.x > -(w2 + sphere->getRadius()))
//			{
//				numContacts++;
//				contact += glm::vec2(-w2, localPos.y);
//				direction = new glm::vec2(-box->getLocalX());
//			}
//		}
//		if (localPos.x < w2 && localPos.x > -w2)
//		{
//			if (localPos.y > 0 && localPos.y < h2 + sphere->getRadius())
//			{
//				numContacts++;
//				contact += glm::vec2(localPos.x, h2);
//				direction = new glm::vec2(box->getLocalY());
//			}
//			if (localPos.y < 0 && localPos.y > -(h2 + sphere->getRadius()))
//			{
//				numContacts++;
//				contact += glm::vec2(localPos.x, -h2);
//				direction = new glm::vec2(-box->getLocalY());
//			}
//		}
//		if (numContacts > 0)
//		{
//			// average, and convert back into world coords
//			contact = box->getPosition() + (1.0f / numContacts) *
//				(box->getLocalX() * contact.x + box->getLocalY() * contact.y);
//
//			// with the contact point we can find a penetration vector
//			float pen = sphere->getRadius() - glm::length(contact -
//				sphere->getPosition());
//			glm::vec2 penVec = glm::normalize(contact - sphere->getPosition()) * pen;
//
//			// move each shape away in the direction of penitration
//			if (!box->isKinematic() && !sphere->isKinematic())
//			{
//				box->setPosition(box->getPosition() + penVec * 0.5f);
//				sphere->setPosition(sphere->getPosition() - penVec * 0.5f);
//			}
//			else if (!box->isKinematic())
//			{
//				box->setPosition(box->getPosition() + penVec);
//			}
//			else
//			{
//				sphere->setPosition(sphere->getPosition() - penVec);
//			}
//
//			box->resolveCollision(sphere, contact, direction);
//		}
//		delete direction;
//	}
//	return false;
//}
//
//// test collision between 2 boxes
//bool PhysicsScene::box2Box(PhysicsObject* a, PhysicsObject* b)
//{
//	Box* box1 = dynamic_cast<Box*>(a);
//	Box* box2 = dynamic_cast<Box*>(b);
//
//	if (box1 != nullptr && box2 != nullptr)
//	{
//		glm::vec2 boxPos = box2->getCenter() - box1->getCenter();
//
//		glm::vec2 normal(0, 0);
//		glm::vec2 contactForce1(0);
//		glm::vec2 contactForce2(0);
//		glm::vec2 contact(0);
//		int numContacts = 0;
//
//		box1->checkBoxCorners(*box2, contact, numContacts, normal, contactForce1);
//
//		if (box2->checkBoxCorners(*box1, contact, numContacts, normal, contactForce2))
//		{
//			normal = -normal;
//		}
//		if (numContacts > 0)
//		{
//			glm::vec2 contactForce = 0.5f * (contactForce1 - contactForce2);
//
//			box1->setPosition(box1->getPosition() - contactForce);
//			box2->setPosition(box2->getPosition() + contactForce);
//
//			box1->resolveCollision(box2, contact / float(numContacts), &normal);
//			return true;
//		}
//	}
//	return false;
//}
//
//// test collision between a box and an axis aligned bounding box
//bool PhysicsScene::box2AABB(PhysicsObject* a, PhysicsObject* b)
//{
//	return false;
//}
//
//// test collision between an axis aligned bounding box and a plane
//bool PhysicsScene::AABB2Plane(PhysicsObject* a, PhysicsObject* b)
//{
//	// try to cast objects to Aabb and Plane
//	Aabb* aabb = dynamic_cast<Aabb*>(a);
//	Plane* plane = dynamic_cast<Plane*>(b);
//
//	// if we are successful then test for collision
//	if (aabb != nullptr && plane != nullptr)
//	{
//		glm::vec2 collisionNormal = plane->getNormal();
//
//		float cornerOffsets[4];
//		bool signs[4];
//
//		for (int i = 0; i < 4; i++)
//		{
//			cornerOffsets[i] = glm::dot(aabb->getCorner(i + 1), collisionNormal) - plane->getDistance();
//			signs[i] = std::signbit(cornerOffsets[i]);
//		}
//
//		if (signs[0] != signs[3] || signs[1] != signs[2])
//		{
//			plane->resolveCollision(aabb, glm::vec2(0));
//			return true;
//		}
//	}
//	return false;
//}
//
//// test collision between an axis aligned bounding box and a sphere
//bool PhysicsScene::AABB2Sphere(PhysicsObject* a, PhysicsObject* b)
//{
//	// try to cast objects to Aabb and sphere
//	Aabb* aabb = dynamic_cast<Aabb*>(a);
//	Sphere* sphere = dynamic_cast<Sphere*>(b);
//
//	// if we are successful then test for collision
//	if (aabb != nullptr && sphere != nullptr)
//	{
//		glm::vec2 boxToSphere;
//		boxToSphere.x = sphere->getPosition().x - fmaxf(aabb->getPosition().x - aabb->getWidth() / 2, fminf(sphere->getPosition().x, aabb->getPosition().x + aabb->getWidth() / 2));
//		boxToSphere.y = sphere->getPosition().y - fmaxf(aabb->getPosition().y - aabb->getHeight() / 2, fminf(sphere->getPosition().y, aabb->getPosition().y + aabb->getHeight() / 2));
//
//		if (glm::length(boxToSphere) < sphere->getRadius())
//		{
//			// collision
//			glm::vec2 collisionNormal = glm::normalize(boxToSphere);
//			float penetration = fabsf(glm::length(boxToSphere) - sphere->getRadius());
//
//			aabb->setPosition(aabb->getPosition() - collisionNormal * penetration);
//			sphere->setPosition(sphere->getPosition() + collisionNormal * penetration);
//
//			glm::vec2 contact = sphere->getPosition() - boxToSphere;
//			aabb->resolveCollision(sphere, glm::vec2(0));
//			return true;
//		}
//	}
//
//	return false;
//}
//
//// test collision between an axis aligned bounding box and a box
//bool PhysicsScene::AABB2Box(PhysicsObject* a, PhysicsObject* b)
//{
//	return false;
//}
//
//// test collision between 2 axis aligned bounding boxes
//bool PhysicsScene::AABB2AABB(PhysicsObject* a, PhysicsObject* b)
//{
//	Aabb* Aabb1 = dynamic_cast<Aabb*>(a);
//	Aabb* Aabb2 = dynamic_cast<Aabb*>(b);
//
//	if (Aabb1 != nullptr && Aabb2 != nullptr)
//	{
//		glm::vec2 aMin = Aabb1->getMin();
//		glm::vec2 aMax = Aabb1->getMax();
//
//		glm::vec2 bMin = Aabb2->getMin();
//		glm::vec2 bMax = Aabb2->getMax();
//
//		if (aMax.x < bMin.x || aMin.x > bMax.x) return false;
//		if (aMax.y < bMin.y || aMin.y > bMax.y) return false;
//
//		float deltaX = (Aabb1->getExtents().x + Aabb2->getExtents().x) - fabsf(Aabb1->getPosition().x - Aabb2->getPosition().x);
//		float deltaY = (Aabb1->getExtents().y + Aabb2->getExtents().y) - fabsf(Aabb1->getPosition().y - Aabb2->getPosition().y);
//
//		glm::vec2 intersection(deltaX, deltaY);
//		float penetration = glm::length(intersection);
//
//		Aabb1->resolveCollision(Aabb2, glm::vec2(0));
//		Aabb1->correctPosition(Aabb2, penetration);
//
//		return true;
//	}
//
//	return false;
//}