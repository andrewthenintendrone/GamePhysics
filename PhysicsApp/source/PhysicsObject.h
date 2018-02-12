#pragma once
#include <glm\vec2.hpp>

enum ShapeType
{
	PLANE = 0,
	SPHERE,
};

const int SHAPE_COUNT = 2;

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {};

public:

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

	virtual ShapeType getShapeID();

protected:
	ShapeType m_shapeID;
};