#pragma once
#include "PhysicsObject.h"
#include <glm/glm.hpp>


class Rigidbody : public PhysicsObject
{
public:

	Rigidbody(ShapeType shapeID, glm::vec2 position,
		glm::vec2 velocity, float orientation, float mass);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void ApplyForce(glm::vec2 force);
	void ApplyForceToActor(Rigidbody* actor2, glm::vec2 force);

	void ResolveCollision(Rigidbody* actor2);

	glm::vec2 GetPosition() { return m_position; }
	float GetOrientation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }

	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }

	void SetVelocity(const glm::vec2 velocity) { m_velocity = velocity; }


	float GetMass() { return m_mass; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	glm::vec2 m_gravity;
	float m_mass;
	float m_orientation;
	glm::vec4 m_color;
};

