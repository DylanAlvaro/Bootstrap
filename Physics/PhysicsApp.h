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
	void PlayerTurns();


protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
	
	std::vector<PhysicsObject*> m_objects;
	std::vector<Circle*> m_balls;
	std::vector<Circle*> m_inHole;

	Circle* cueBall;
	Circle* blackBall;
	Box* staticBox;

	glm::vec2 m_position;
	
	//glm::vec2 m_localX;
	//glm::vec2 m_localY;
	//
	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;

	float m_cameraX;
	float m_cameraY;

	bool hasBeenHit = false;

	bool hasBeenSunk;
	bool m_isAsleep = false;

	//glm::vec2 hitpos;

	int m_playersTurn;

	void OnBall2Check(PhysicsObject* other);
	void SpawnBalls();



public:
	void DemoStartUp(int num);
	void DemoUpdates(aie::Input* input, float dt);
	float DegreeToRadian(float degree);
};