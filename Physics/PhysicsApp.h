#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"
#include <vector>

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

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
	
	std::vector<PhysicsObject*> m_objects;

public:
	void DemoStartUp(int num);
	void DemoUpdates(aie::Input* input, float dt);
	float DegreeToRadian(float degree);
};