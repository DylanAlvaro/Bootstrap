#include "Plane.h"
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <iostream>

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(ShapeType::PLANE)
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

void Plane::Draw(float alpha)
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	// easy to rotate normal through 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colorFade = m_color;
	colorFade = glm::vec4(1, 1, 1, 1);
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, colorFade);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_color, m_color, colorFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_color, colorFade, colorFade);
}

void Plane::ResetPosition()
{
}

float Plane::GetKineticEnergy()
{
	return 0.0f;
}

void Plane::ResolveCollision(Rigidbody* actor2)
{
	glm::vec2 relativeVelocity = actor2->GetVelocity();

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), m_normal) / (1 / actor2->GetMass());

	glm::vec2 force = GetNormal() * j;

	float kePre = actor2->GetKineticEnergy();

	actor2->ApplyForce(force);

	float kePost = actor2->GetKineticEnergy();

	float deltaKE = kePost - kePre;
	if (deltaKE > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepnacy greater than 1% detected!!";
}
