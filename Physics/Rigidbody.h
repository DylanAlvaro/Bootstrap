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
	
	//getters
	glm::vec2 GetPosition() { return m_position; }
	float GetOrientation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }

	virtual float GetKineticEnergy();

	float GetPotentialEnergy();

	

	//setters
	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void SetOrientation(float orientation) { m_orientation = orientation; }
	void SetMass(float mass) { m_mass = mass; }



protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	glm::vec2 m_gravity;
	glm::vec4 m_color;
	
	
	float m_mass;
	float m_kinetic;
	float m_orientation;
	float m_angularVelocity;
	float m_moment;
	float m_lastOrientation;
	//float m_potentialEnergy;

};

