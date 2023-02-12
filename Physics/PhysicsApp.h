#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"
#include <vector>
#include "Circle.h"
#include "Box.h"

class PhysicsScene;
class PhysicsObject;

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	glm::vec2 ScreenToWorld(glm::vec2 screenPos);

	int m_playersTurn;

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
	
	std::vector<PhysicsObject*> m_objects;

	Circle* cueBall;
	Box* staticBox;

	glm::vec2 m_position;
	glm::vec2 m_localX;
	glm::vec2 m_localY;


public:
	void DemoStartUp(int num);
	void DemoUpdates(aie::Input* input, float dt);
	float DegreeToRadian(float degree);
};