#include "Box.h"

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extends, glm::vec4 color) : Rigidbody(BOX, position, velocity, 0, mass)
{
	m_position = position;
	m_color = color;
	m_extends = extends;
	m_moment = 1.0f / 12.0f * mass * extends.x * extends.y;
}

void Box::Draw(float alpha)
{

}
