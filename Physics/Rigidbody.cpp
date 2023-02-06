#include "Rigidbody.h"
#include <iostream>
#include "PhysicsScene.h"


Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass) : PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_orientation = orientation;
	m_lastPosition = m_position;
	m_angularVelocity = 0.f;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep, glm::vec2(0));

	m_orientation += m_angularVelocity * timeStep;
}

void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / GetMass();
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / GetMoment();
}

void Rigidbody::ApplyForceToActor(Rigidbody* actor2, glm::vec2 force)
{
	actor2->ApplyForce(force, glm::vec2(0));
	ApplyForce(-force, glm::vec2(0));
}

void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->m_position - m_position);

	glm::vec2 perp(normal.y - normal.x);
	
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);

	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;

	float v2 = glm::dot(actor2->m_position, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2)
	{
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);

		float elasticity = 1;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		//apply equal and opposite forces
		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->m_position);
	}

	//float elasticity = 1;
	//float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / ((1 / GetMass()) + (1 / actor2->GetMass()));
	//
	//glm::vec2 force = normal * j;
	//
	//float kePre = GetKineticEnergy() + actor2->GetKineticEnergy();
	//
	//ApplyForceToActor(actor2, force);
	//
	//float kePost = GetKineticEnergy() + actor2->GetKineticEnergy();
	//
	//float deltaKE = kePost - kePre;
	//if (deltaKE > kePost * 0.01f)
	//	std::cout << "Kinetic Energy discrepnacy greater than 1% detected!!";
}
float PhysicsScene::GetTotalEnergy()
{
	float total = 0;
	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
	{
		PhysicsObject* obj = *it;
		total += obj->GetEnergy();
	}
	return total;
}

float Rigidbody::GetKineticEnergy()
{
	m_kinetic = (m_mass * 0.5f) * ((pow(m_velocity.x, 2) * pow(m_velocity.y, 2)));
	return m_kinetic; 
}

float Rigidbody::GetPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
}

void Rigidbody::CalculateSmoothedPosition(float alpha)
{
	m_smoothedPosition = alpha * m_position + (1 - alpha) * m_lastPosition;

	float smoothedOrientation = alpha * m_orientation
		+ (1 - alpha) * m_lastOrientation;
	
	float cs = cosf(smoothedOrientation);
	float sn = sinf(smoothedOrientation);
	
	m_smoothedLocalX = glm::vec2(cs, sn);
	m_smoothedLocalY = glm::vec2(-sn, cs);
}

void Rigidbody::CalculateAxes()
{
	float sn = sinf(m_orientation);
	float cs = cosf(m_orientation);

	m_localX = glm::vec2(cs, sn);
	m_localY = glm::vec2(-sn, cs);
}

float Rigidbody::GetEnergy()
{
	return GetKineticEnergy() + GetPotentialEnergy();
}

