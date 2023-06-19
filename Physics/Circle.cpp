#include "Circle.h"
#include "Rigidbody.h"
#include "Gizmos.h"

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color) : Rigidbody(CIRCLE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_color = color;
	m_moment = 0.5f * mass * radius * radius;
	m_isKinematic = false;
	
	if (m_isPocket)
		m_isKinematic = true;
}

Circle::~Circle()
{
}

void Circle::Draw(float alpha)
{
	CalculateSmoothedPosition(alpha);

	aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 24, m_color);

	//aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(1, 1, 1, 1));
}
