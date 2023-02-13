#pragma once
#include "PhysicsObject.h"
#include <glm/glm.hpp>
#include <list>
#include <functional>


class Rigidbody : public PhysicsObject
{
public:

	const int MIN_LINEAR_THRESHOLD = 0.1f;
	const int MIN_ANGULAR_THRESHOLD = 0.01f;

	Rigidbody(ShapeType shapeID, glm::vec2 position,
		glm::vec2 velocity, float orientation, float mass);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	
	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	void ApplyForceToActor(Rigidbody* actor2, glm::vec2 force);
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr, float pen = 0);
	void CalculateAxes();

	//getters
	glm::vec2 GetPosition() const { return m_position; }
	float GetOrientation()  { return m_orientation; }
	float GetAngularVelocity() { return m_angularVelocity; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass()  { return m_isKinematic ? INT_MAX : m_mass; }
	float GetMoment() { return m_isKinematic ? INT_MAX : m_moment; }
	virtual float GetKineticEnergy();
	float GetPotentialEnergy();

	float GetElasticity() { return m_elasticity; }

	glm::vec2 GetLocalX() const { return m_localX; }
	glm::vec2 GetLocalY() const { return m_localY; }

	void CalculateSmoothedPosition(float alpha);

	virtual float GetEnergy();

	glm::vec2 ToWorld(glm::vec2 contact);
	glm::vec2 ToWorldSmoothed(glm::vec2 localPos);

	std::function<void(PhysicsObject*)> collisionCallback;

	//setters
	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void SetOrientation(float orientation) { m_orientation = orientation; }
	void SetMass(float mass) { m_mass = mass; }
	void SetPosition(const glm::vec2 position) { m_position = position; }
	void SetKinematic(bool state) { m_isKinematic = state; }
	bool IsKinematic() { return m_isKinematic; }
	void SetElasticity(float elasticity) { m_elasticity = elasticity; }
	void SetTrigger(bool state) { m_isTrigger = state; }
	float GetLinearDrag() { return m_linearDrag; }
	float GetAngularDrag() { return m_angularDrag; }

	bool IsTrigger() { return m_isTrigger; }
	void TriggerEnter(PhysicsObject* actor2);

	std::function<void(PhysicsObject*)> triggerEnter;
	std::function<void(PhysicsObject*)> triggerExit;

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


	

	float m_linearDrag = 0.3f;
	float m_angularDrag = 0.3f;

	bool m_isKinematic;
	bool m_isTrigger;
	std::list<PhysicsObject*> m_objectsInside;
	std::list<PhysicsObject*> m_objectsInsideThisFrame;
};

