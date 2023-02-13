#include "Rigidbody.h"
#include <iostream>
#include "PhysicsScene.h"
#include "Demo.h"


Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass) : PhysicsObject(shapeID)
{
	m_position = position;
	m_orientation = orientation;
	m_lastPosition = m_position;
	m_lastOrientation = m_orientation;

	m_velocity = velocity;
	m_mass = mass;
	m_angularVelocity = 0;
	m_elasticity = 0.3f;
	m_isTrigger = false;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	CalculateAxes();

	if (m_isTrigger)
	{
		for (auto it = m_objectsInside.begin(); it != m_objectsInside.end(); it++)
		{
			if (std::find(m_objectsInsideThisFrame.begin(), m_objectsInsideThisFrame.end(), *it) == m_objectsInsideThisFrame.end())
			{
				if (triggerExit)
					triggerExit(*it);

				it = m_objectsInside.erase(it);
				if (it == m_objectsInside.end())
					break;
			}
		}
	}
	m_objectsInsideThisFrame.clear();

	if (m_isKinematic)
	{
		m_velocity = glm::vec2(0);
		m_angularVelocity = 0;
		return;
	}

	m_lastPosition = m_position;
	m_lastOrientation = m_orientation;


	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD) 
	{
		m_angularVelocity = 0;
	}

	m_position += m_velocity * timeStep;
	ApplyForce(gravity * GetMass() * timeStep, glm::vec2(0));

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
	return 0.5f * (GetMass() * glm::dot(m_velocity, m_velocity) + GetMoment() * m_angularVelocity * m_angularVelocity);
}

float Rigidbody::GetPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
}

void Rigidbody::CalculateSmoothedPosition(float alpha)
{
	m_smoothedPosition = alpha * m_position + (1 - alpha) * m_lastPosition;

	float smoothedOrientation = alpha * m_orientation + (1 - alpha) * m_lastOrientation;

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

glm::vec2 Rigidbody::ToWorld(glm::vec2 contact)
{
	return m_position + m_localX * contact.x + m_localY * contact.y;
}

glm::vec2 Rigidbody::ToWorldSmoothed(glm::vec2 localPos)
{
	return m_smoothedPosition + m_smoothedLocalX * localPos.x + m_smoothedLocalY * localPos.y;
}
void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal, float pen)
{
	m_objectsInsideThisFrame.push_back(actor2);
	actor2->m_objectsInsideThisFrame.push_back(this);

	// find the vector between their centres, or use the provided direction
	// of force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->m_position - m_position);
	glm::vec2 relativeVelocity = actor2->GetVelocity() - m_velocity;

	// get the vector perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);

	// determine the total velocity of the contact points for the two objects, 
	// for both linear and rotational 		

		// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);
	// velocity of the contact point on this object 
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// velocity of contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2) // they're moving closer
	{
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.
		float mass1 = 1.0f / (1.0f / GetMass() + (r1 * r1) / GetMoment());
		float mass2 = 1.0f / (1.0f / actor2->GetMass() + (r2 * r2) / actor2->GetMoment());

		float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		float kePre = GetKineticEnergy() + actor2->GetKineticEnergy();

		//Apply equal and opposite forces
		
		if (!m_isTrigger && !actor2->m_isTrigger)
		{
			ApplyForce(-force, contact - m_position);
			actor2->ApplyForce(force, contact - actor2->m_position);

			if (collisionCallback != nullptr)
				collisionCallback(actor2);
			if (actor2->collisionCallback)
				actor2->collisionCallback(this);
		}
		else
		{
			TriggerEnter(actor2);
			actor2->TriggerEnter(this);
		}

		float kePost = GetKineticEnergy() + actor2->GetKineticEnergy();

		float deltaKE = kePost - kePre;
		if (deltaKE > kePost * 0.01f)
			std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";

		if (pen > 0)
			PhysicsScene::ApplyContactForces(this, actor2, normal, pen);
	}

		//apply equal and opposite forces
}

void Rigidbody::TriggerEnter(PhysicsObject* actor2)
{
	if (m_isTrigger && std::find(m_objectsInside.begin(), m_objectsInside.end(), actor2) == m_objectsInside.end())
	{
		m_objectsInside.push_back(actor2);
		if (triggerEnter != nullptr)
			triggerEnter(actor2);
	}
}




