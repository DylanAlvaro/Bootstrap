#include "Plane.h"
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <iostream>
#include "PhysicsScene.h"

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(ShapeType::PLANE)
{
	m_normal = glm::normalize(normal);
	m_distanceToOrigin = distance;
	m_elasticity = .4;
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::Draw(float alpha)
{
	m_color = glm::vec4(0.3f, 0.3f, 0.3f, 1);

	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	// easy to rotate normal through 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colorFade = m_color;
	colorFade = glm::vec4(1, 1, 1, 1);
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	
	aie::Gizmos::add2DLine(start, end, m_color);
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

void Plane::ResolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
	// the position at which we'll apply the force relative to the object's COM
	glm::vec2 localContact = contact - actor2->GetPosition();

	// the plane isn't moving, so the relative velocity is just actor2's velocity at the contact point
	glm::vec2 vRel = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = glm::dot(vRel, m_normal);

	// average the elasticity of the plane and the rigidbody
	float e = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

	// this is the perpendicular distance we apply the force at relative to the COM, so Torque = F*r
	float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));

	// work out the "effective mass" - this is a combination of moment of
	// inertia and mass, and tells us how much the contact point velocity 
	// will change with the force we're applying
	float mass0 = 1.0f / (1.0f / actor2->GetMass() + (r * r) / actor2->GetMoment());

	float j = -(1 + e) * velocityIntoPlane * mass0;

	glm::vec2 force = m_normal * j;

	float kePre = actor2->GetKineticEnergy();

	actor2->ApplyForce(force, contact - actor2->GetPosition());

	if (actor2->collisionCallback)
		actor2->collisionCallback(this);


	float pen = glm::dot(contact, m_normal) - m_distanceToOrigin;
	PhysicsScene::ApplyContactForces(actor2, nullptr, m_normal, pen);


	float kePost = actor2->GetKineticEnergy();

	float deltaKE = kePost - kePre;
	if (deltaKE > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";
}