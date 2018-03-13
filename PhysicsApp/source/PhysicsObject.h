#pragma once
#include <glm\glm.hpp>

enum ShapeTypes
{
	PLANE = 0,
	SPHERE,
	AABB,
	BOX,
	SHAPECOUNT
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeTypes shapeID) : m_shapeID(shapeID) {};

public:

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void draw() = 0;

	virtual ShapeTypes getShapeID() const{ return m_shapeID; }

protected:
	ShapeTypes m_shapeID;
};