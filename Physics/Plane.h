#pragma once
#include "PhysicsObject.h"
#include <glm/glm.hpp>
#include "Rigidbody.h"

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 normal, float distance);
	~Plane();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Draw(float alpha);
	virtual void ResetPosition();

	glm::vec2 GetNormal() { return m_normal; }
	float GetDistance() { return m_distanceToOrigin; }

	virtual float GetKineticEnergy();

	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact);

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
	glm::vec4 m_color;
};

