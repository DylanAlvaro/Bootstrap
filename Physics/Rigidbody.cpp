#include "Rigidbody.h"


Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass) : PhysicsObject(m_shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_orientation = orientation;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep);
}

void Rigidbody::ApplyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::ApplyForceToActor(Rigidbody* actor2, glm::vec2 force)
{
	actor2->ApplyForce(force);
	ApplyForce(-force);
}
