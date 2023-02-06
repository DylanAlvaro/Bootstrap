#pragma once
#include <glm/glm.hpp>

enum ShapeType {
	PLANE = 0,
	CIRCLE,
	//BOX
	SHAPE_COUNT,
};

class PhysicsObject {
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {};
public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;

	virtual void Draw(float alpha) = 0;
	virtual void ResetPosition() {};

	virtual float GetEnergy() { return 0.0f; }

	ShapeType GetShapeID() { return m_shapeID; }

protected:
	ShapeType m_shapeID;
};

