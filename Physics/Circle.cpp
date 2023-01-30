#include "Circle.h"
#include "Rigidbody.h"
#include "Gizmos.h"

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color) : Rigidbody(CIRCLE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_color = color;
}

void Circle::Draw()
{
	aie::Gizmos::add2DCircle(glm::vec2(m_position), m_radius, 13, m_color);
}
