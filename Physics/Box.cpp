#include "Box.h"
#include "Gizmos.h"
#include "Rigidbody.h"

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extends, glm::vec4 color) : Rigidbody(BOX, position, velocity, 0, mass)
{
	m_position = position;
	m_color = color;
	m_extends = extends;
	m_moment = 1.0f / 12.0f * mass * extends.x * extends.y;
}

void Box::Draw(float alpha)
{
	CalculateSmoothedPosition(alpha);

	glm::vec2 p1 = m_smoothedPosition - m_smoothedLocalX * m_extends.x
		- m_smoothedLocalY * m_extends.y;
	glm::vec2 p2 = m_smoothedPosition + m_smoothedLocalX * m_extends.x
		- m_smoothedLocalY * m_extends.y;
	glm::vec2 p3 = m_smoothedPosition - m_smoothedLocalX * m_extends.x
		+ m_smoothedLocalY * m_extends.y;
	glm::vec2 p4 = m_smoothedPosition + m_smoothedLocalX * m_extends.x
		+ m_smoothedLocalY * m_extends.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_color);
	aie::Gizmos::add2DTri(p1, p4, p3, m_color);
}
