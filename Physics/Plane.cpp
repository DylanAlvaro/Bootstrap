#include "Plane.h"

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(m_shapeID)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::Draw()
{
}

void Plane::ResetPosition()
{
}
