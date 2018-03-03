#pragma once
#include <glm\vec2.hpp>

enum ShapeTypes
{
	PLANE = 0,
	SPHERE,
	POLYGON,
	SHAPECOUNT
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeTypes a_shapeID) : m_shapeID(a_shapeID) {};

public:

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;

	virtual ShapeTypes getShapeID() const{ return m_shapeID; }

protected:
	ShapeTypes m_shapeID;
};