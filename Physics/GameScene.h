#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"
#include <vector>
#include "Circle.h"
#include "Box.h"

class PhysicsScene;
class PhysicsObject;

class GameScene : public aie::Application
{
public:

	GameScene();
	~GameScene();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void SetBallPositions();
	void AssignBallType();

	glm::vec2 ScreenToWorld(glm::vec2 screenPos);

protected:
	aie::Renderer2D* m_2dRenderer;
	aie::Font* m_font;

	PhysicsScene* m_physicsScene;

	std::vector<PhysicsObject*> m_objects;

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;


public:
	Circle* cueBallW;
	Circle* cueBallB;

	std::vector<Circle*> m_balls;
	std::vector<Circle*> m_yellow;
	std::vector<Circle*> m_red;
	std::vector<Circle*> m_sunk;
	float DegreeToRadian(float degree);



	int m_playersTurn;
	bool m_ballTypeAssigned;
};

