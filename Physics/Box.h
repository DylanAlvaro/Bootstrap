#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity,
		float mass, glm::vec2 extends, glm::vec4 color);
	~Box();

	virtual void Draw(float alpha);
	bool CheckBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);

	glm::vec4 GetColor() const { return m_color; }
	glm::vec2 GetExtents() const { return m_extents; }
	glm::vec2 GetLocalX()const { return m_localX; }
	glm::vec2 GetLocalY()const { return m_localY; }
	
	float GetHeight() const { return m_extents.y * 2; }
	float GetWidth()  const { return m_extents.x * 2; }
	
protected:

	glm::vec2 m_extents;

	
};

