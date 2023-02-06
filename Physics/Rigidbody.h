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
	
	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	void ApplyForceToActor(Rigidbody* actor2, glm::vec2 force);
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr);
	void CalculateAxes();

	//getters
	glm::vec2 GetPosition() { return m_position; }
	float GetOrientation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetMoment() { return m_moment; }
	virtual float GetKineticEnergy();
	float GetPotentialEnergy();

	void CalculateSmoothedPosition(float alpha);

	virtual float GetEnergy();

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
	
	glm::vec2 m_lastPosition;
	
	glm::vec2 m_smoothedPosition;
	glm::vec2 m_smoothedLocalX;
	glm::vec2 m_smoothedLocalY;

	glm::vec2 m_localX;
	glm::vec2 m_localY;

	
	float m_mass;
	float m_kinetic;
	float m_orientation;
	float m_angularVelocity;
	float m_moment;
	float m_lastOrientation;
	//float m_potentialEnergy;

};

