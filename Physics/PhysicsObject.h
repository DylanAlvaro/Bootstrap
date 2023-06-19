#pragma once
#include <glm/glm.hpp>

enum ShapeType {
	JOINT = -1,
	PLANE = 0,
	CIRCLE,
	BOX,
};
const int SHAPE_COUNT = 3;
class PhysicsObject {
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {};
public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;

	virtual void Draw(float alpha) = 0;
	virtual void ResetPosition() {};

	virtual float GetEnergy() { return 0.0f; }
	float GetElasticity() { return m_elasticity; }

	//virtual bool IsInside(glm::vec2 point) { return false; };

	void SetElasticity(const float elasticity) { m_elasticity = elasticity; }

	ShapeType GetShapeID() { return m_shapeID; }

protected:
	ShapeType m_shapeID;

	float m_elasticity = 0.7;
};

