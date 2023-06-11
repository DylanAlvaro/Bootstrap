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
	glm::vec2 WorldToScreen(glm::vec2 mousePos);
	bool HasPlayerTurnEnded();
	void CheckBallHit();
	void CheckHit();


protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
	
	std::vector<PhysicsObject*> m_objects;
	std::vector<Circle*> m_balls;
	std::vector<Circle*> m_inHole;
	std::vector<Circle*> m_sunkBallsSolids;
	std::vector<Circle*> m_sunkBallStripes;
	std::vector<Circle*> m_sunkWhite;
	std::vector<Circle*> m_whiteBall;

	std::vector<Circle*> m_stripes;
	std::vector<Circle*> m_solids;

	Circle* cueBall;
	Circle* blackBall;
	Box* staticBox;
	Box* m_trigger;
	glm::vec4 m_triggerColor = glm::vec4(0, 0, 0, 1);

	glm::vec2 m_position;
	
	//glm::vec2 m_localX;
	//glm::vec2 m_localY;
	//
	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;

	float m_cameraX;
	float m_cameraY;

	float m_ExtraMove = 0;

	bool hasBeenHit = false;
	glm::vec2 m_hitForce;

	bool isWhiteBallPlaced = true;

	bool hasBlackSunk = false;

	bool hasBeenSunk = false;
	bool m_player1Win = false;
	bool m_player2Win = false;
	bool sunkCheck = false;
	
	bool m_isAsleep = false;
	bool m_player1turn = true;
	bool m_player2turn = true;
	float m_player1Score = 0;
	float m_player2Score = 0;

	//glm::vec2 hitpos;


	int m_playersTurn;
	void OnBall2Check(PhysicsObject* other);
	void SpawnBalls();

	void HasBeenSunk();


public:
	void DemoStartUp(int num);
	void DemoUpdates(aie::Input* input, float dt);
	float DegreeToRadian(float degree);
};