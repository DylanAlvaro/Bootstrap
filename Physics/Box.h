#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity,
		float mass, glm::vec2 extends, glm::vec4 color);
	~Box();

	virtual void Draw(float alpha);

	glm::vec4 GetColor() { return m_color; }
	glm::vec2 GetExtents() { return m_extends; }


protected:

	glm::vec2 m_extends;

};

