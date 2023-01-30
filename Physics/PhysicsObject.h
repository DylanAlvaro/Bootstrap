#pragma once
#include <glm/vec2.hpp>

class PhysicsObject
{
protected:
	PhysicsObject(){}

public:
	virtual void FixedUpdate(glm::vec2 gravity, 
		float timeStep) = 0;

	virtual void Draw(float alpha) = 0;
	virtual void ResetPosition() {};


};

