#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Demo.h"
#include "Circle.h"
#include "Plane.h"

#include "PhysicsScene.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <glm/vec2.hpp>

#include "Rigidbody.h"
#include "Box.h"
#include "Spring.h"
#include <iostream>
#include <list>



PhysicsApp::PhysicsApp() 
{
	
}

PhysicsApp::~PhysicsApp() 
{

}

bool PhysicsApp::startup()
{
	//increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();


	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01f);

	DemoStartUp(1);
	return true;
}

void PhysicsApp::shutdown() 
{

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime)
{

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	DemoUpdates(input, deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	for (int i = 0; i < m_inHole.size(); i++)
	{
		m_inHole[i]->triggerEnter = [=](PhysicsObject* other)
		{
			if (other != cueBall)
			{
				if (other != blackBall)
				{
					for (int j = 0; j < m_balls.size(); j++)
					{
						if (other == m_balls[j])
						{
							// checking if player 1 sunk a ball
							if (m_balls[j]->GetSunkBallCheck() && !m_player1turn)
							{
								m_ExtraMove++;
								m_player1turn = true;
							}
							// checking if player 2 sunk a ball
							if (!m_balls[j]->GetSunkBallCheck() && m_player1turn)
							{
								m_ExtraMove++;
								m_player1turn = false;
							}

							m_balls[j]->SetKinematic(true);
							
							// Checking which player sunk the ball
							if (hasBeenSunk)
							{
								// if player 1 sunk the ball their score increase and they get an extra move
								if (m_balls[j]->GetSunkBallCheck())
								{
									m_balls[j]->SetPosition(glm::vec2(-10 - m_player1Score, 500));
									m_player1Score += 10;
									m_ExtraMove++;
								}

								// if player 2 sunk the ball their score increase and they get an extra move
								if (m_balls[j]->GetSunkBallCheck())
								{
									m_balls[j]->SetPosition(glm::vec2(10 + m_player2Score, 500));
									m_player2Score += 10;
									m_ExtraMove++;
								}
							}

							if (!hasBeenSunk)
							{
								if (m_player1turn)
								{
									m_balls[j]->SetPosition(glm::vec2(-10 - m_player1Score, 500));
									m_player1Score += 10;
									m_ExtraMove++;
								}
								if (!m_player1turn)
								{
									m_balls[j]->SetPosition(glm::vec2(10 + m_player2Score, 500));
									m_player2Score += 10;
									m_ExtraMove++;
								}

								// Now going to check which player sunk a stripe
								if (m_balls[j]->GetStripe())
								{
									if (m_player1turn)
									{
										for (int i = 0; i < m_stripes.size(); i++)
										{
											m_stripes[i]->SetSunkBallCheck(true);
										}

										for (int x = 0; x < m_solids.size(); x++)
										{
											hasBeenSunk = true;
										}

									}
									if (!m_player1turn)
									{
										for (int b = 0; b < m_solids.size(); b++)
										{
											m_solids[b]->SetSunkBallCheck(true);
										}
										hasBeenSunk = true;
									}
								}
								// Now going to check which player sunk a solid
								if (!m_balls[j]->GetStripe())
								{
									if (m_player1turn)
									{
										for (int b = 0; b < m_solids.size(); b++)
										{
											m_solids[b]->SetSunkBallCheck(true);
										}
										hasBeenSunk = true;
									}
									if (m_player2turn) 
									{
										for (int a = 0; a < m_stripes.size(); a++)
										{
											m_stripes[a]->SetSunkBallCheck(true);
										}
										hasBeenSunk = true;
									}
								}
							}
						}
					}
				}
				else
				{
				blackBall->SetPosition(glm::vec2(200, 200));
				if (!m_player1turn && m_player1Score == 70 && m_player2Score == 70)
					m_player2Win = true;
				if (m_player1turn && m_player1Score == 70 && m_player2Score == 70)
					m_player1Win = true;
				if (m_player1turn && m_player1Score == 70 && m_player2Score != 70)
					m_player1Win = true;
				if (!m_player1turn && m_player1Score != 70 && m_player2Score == 70)
					m_player2Win = true;
				if (!m_player1turn && m_player2Score != 70)
					m_player1Win = true;
				if (m_player1turn && m_player1Score != 70)
					m_player2Win = true;
				}
			}
			else
			{
				cueBall->SetVelocity(glm::vec2(0));
				cueBall->SetPosition(glm::vec2(-4000, -500));
				isWhiteBallPlaced = false;
			}
		};
	}

	// This is going to let the user place the white ball in an area when they put the ball into the hole.
	if (!isWhiteBallPlaced)
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
		cueBall->SetPosition(worldPos);

		blackBall->SetKinematic(true);

		if (cueBall->GetPosition().x > -80 &&
			cueBall->GetPosition().x < -25 &&
			cueBall->GetPosition().y > -40 &&
			cueBall->GetPosition().y < 30)
		{
			if (input->wasMouseButtonReleased(1))
			{
				cueBall->SetPosition(worldPos);
				cueBall->SetVelocity(glm::vec2(0));
				isWhiteBallPlaced = true;

				blackBall->SetKinematic(false);

				for (int i = 0; i < m_stripes.size(); i++)
					m_stripes[i]->SetKinematic(false);

				for (int j = 0; j < m_solids.size(); j++)
					m_solids[j]->SetKinematic(false);
			}
		}
	}

	/// <summary>
	/// Checking player turns, if they've sunk the ball and get an extra move or not. 
	/// </summary>
	/// <param name="deltaTime"></param>
	if (m_ExtraMove > 1)
		m_ExtraMove = 1;

	if (HasPlayerTurnEnded() && hasBeenHit && isWhiteBallPlaced)
	{
		hasBeenHit  = false;

		if (m_ExtraMove == 0)
		{
			if (m_player1turn)
			{
				m_player1turn = false;
				sunkCheck = true;
			}

			if (!m_player1turn && !sunkCheck)
			{
				m_player1turn = true;
			}
			if (m_ExtraMove < 1)
				m_ExtraMove = 0;
		}
	}

	if (input->isMouseButtonDown(0) && !hasBeenHit)
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));

		
		if (m_player1turn)
		{
			aie::Gizmos::add2DCircle(worldPos, 2, 32, glm::vec4(1, 1, 0, 1));
			aie::Gizmos::add2DLine(worldPos, cueBall->GetPosition(), glm::vec4(1, 1, 0, 1));
		}

		if (m_player2turn)
		{
			aie::Gizmos::add2DCircle(worldPos, 2, 32, glm::vec4(0, 1, 0, 1));
			aie::Gizmos::add2DLine(worldPos, cueBall->GetPosition(), glm::vec4(0, 1, 0, 1));
		}

		m_hitForce = glm::vec2(cueBall->GetPosition().x - worldPos.x, cueBall->GetPosition().y - worldPos.y);
		CheckHit();
	}

	if (input->wasMouseButtonReleased(0) && !hasBeenHit)
	{
		if (m_ExtraMove > 0)
		{
			m_ExtraMove--;
		}
		CheckBallHit();
		hasBeenHit = true;
	}
}

void PhysicsApp::draw() 
{
	// wipe the screen to the background colour
	clearScreen();
	m_2dRenderer->begin();

	// begin drawing sprites
	m_2dRenderer->drawText(m_font, "solids", 80, 680);
	m_2dRenderer->drawText(m_font, "stripes", 900, 680);


	if (m_player1Win)
	{
		m_2dRenderer->drawText(m_font, "Player 1 wins", 50, 50);
	}


	if (m_player2Win)
	{
		m_2dRenderer->drawText(m_font, "Player 2 wins", 50, 50);
	}

	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	//m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

glm::vec2 PhysicsApp::ScreenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	// move the centre of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// scale according to our extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}

glm::vec2 PhysicsApp::WorldToScreen(glm::vec2 worldPos)
{
	glm::vec2 screenPos = worldPos;
	// scale according to our extents
	screenPos.x /= 2.0f;
	screenPos.x /= m_extents;
	screenPos.x *= getWindowWidth();

	screenPos.y /= 2.0f;
	screenPos.y /= m_extents;
	screenPos.y *= (m_aspectRatio * getWindowHeight());

	screenPos.x += getWindowWidth() / 2;
	screenPos.y += getWindowHeight() / 2;
		
	return screenPos;
}

bool PhysicsApp::HasPlayerTurnEnded()
{
	float speedCheck = 0.5f;

	for (int i = 0; i < m_balls.size(); i++)
	{
		if (m_balls[i]->GetVelocity().x > speedCheck ||
			m_balls[i]->GetVelocity().y > speedCheck ||
			m_balls[i]->GetVelocity().x < -speedCheck ||
			m_balls[i]->GetVelocity().y < -speedCheck)
			return false;
	}
	return true;
}

void PhysicsApp::CheckBallHit()
{
	if(!hasBeenHit)
		cueBall->ApplyForce(glm::vec2(10 * m_hitForce.x, 10 * m_hitForce.y), cueBall->GetPosition());
}

void PhysicsApp::CheckHit()
{
	if (m_hitForce.x > 50)
		m_hitForce.x = 50;
	if (m_hitForce.x < -50)
		m_hitForce.x = -50;
	if (m_hitForce.y > 50)
		m_hitForce.y = 50;
	if (m_hitForce.y < -50)
		m_hitForce.y = -50;
}

void PhysicsApp::DemoStartUp(int num)
{

#ifdef NewtownsFirstLaw
	m_physicsScene->SetGravity(glm::vec2(0));
	
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif 

#ifdef NewtownsSecondLaw
	m_physicsScene->SetGravity(glm::vec2(0, -10));
	
	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif 
#ifdef NewtownsThirdLaw
	m_physicsScene->SetGravity(glm::vec2(-2, 0));

	//activity 3
	Circle* ball1 = new Circle(glm::vec2(-4, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(4, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	
	ball1->ApplyForceToActor(ball2, glm::vec2(-2, 0));

#endif 
#ifdef SimulatingCollision
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	//activity 4
	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	
	ball1->ApplyForce(glm::vec2(30, 0));
	ball2->ApplyForce(glm::vec2(-15, 0));
#endif 

#ifdef CollisionDetection
	
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Plane* plane = new Plane(glm::vec2(0, 10), -30);
	Circle* ball1 = new Circle(glm::vec2(-20, 10), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 10), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(plane);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
#endif 


#ifdef CollisionResolution
	
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Plane* plane1 = new Plane(glm::vec2(-1, 0), -30);
	Plane* plane2 = new Plane(glm::vec2(1, 0), -20);

	Circle* ball1 = new Circle(glm::vec2(20, 10), glm::vec2(10, 0), 5.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 10), glm::vec2(0), 4.0f, 4, glm::vec4(1, 2, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(30, 10), glm::vec2(0), 3.0f, 4, glm::vec4(1, 2, 0, 1));
	Circle* ball4 = new Circle(glm::vec2(25, 10), glm::vec2(0), 6.0f, 4, glm::vec4(1, 2, 0, 1));
	
	Box* box1 = new Box(glm::vec2(10, 10), glm::vec2(0, 0), 0, glm::vec2(5), glm::vec4(1, 1, 1, 1));
	
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);

	m_physicsScene->AddActor(box1);

#endif 

#ifdef RotationalForce2
	
	m_physicsScene->SetGravity(glm::vec2(0, -32.f));

	Plane* plane1 = new Plane(glm::vec2(0, 1), -30);

	Circle* ball1 = new Circle(glm::vec2(-20, 10), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 10), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	Box* box1 = new Box(glm::vec2(-20, 30), glm::vec2(0), 5.0f, glm::vec2(1, 5), glm::vec4(0, 1, 1, 1));
	Box* box2 = new Box(glm::vec2(-25, 10), glm::vec2(0), 4.0f, glm::vec2(2,5), glm::vec4(0, 1, 1, 1));

	m_physicsScene->AddActor(plane1);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);
	
#endif 

#ifdef PoolTable

	m_physicsScene->SetGravity(glm::vec2(0, 0));
	m_physicsScene->SetTimeStep(0.01f);

	// top left
	Box* box1 = new Box(glm::vec2(-55, 45.5f), glm::vec2(0), glm::vec2(0), 10, glm::vec2(2.5f, 35), glm::vec4(0.5f, 0.3f, 0, 1));
	box1->SetOrientation(DegreeToRadian(90));
	box1->SetKinematic(true);
	
	Box* box2 = new Box(glm::vec2(40, 45.5f), glm::vec2(0), glm::vec2(0), 10, glm::vec2(2.5f, 40), glm::vec4(0.5f, 0.3f, 0, 1));
	box2->SetOrientation(DegreeToRadian(90));
	box2->SetKinematic(true);

	Box* box3 = new Box(glm::vec2(-55, -55.5f), glm::vec2(0), glm::vec2(0), 10, glm::vec2(2.5f, 35), glm::vec4(0.5f, 0.3f, 0, 1));
	box3->SetOrientation(DegreeToRadian(90));
	box3->SetKinematic(true);

	Box* box4 = new Box(glm::vec2(40, -55.5f), glm::vec2(0), glm::vec2(0), 10, glm::vec2(2.5f, 40), glm::vec4(0.5f, 0.3f, 0, 1));
	box4->SetOrientation(DegreeToRadian(90));
	box4->SetKinematic(true);

	Box* box5 = new Box(glm::vec2(-99.f, 0), glm::vec2(0), glm::vec2(0), 10, glm::vec2(44, 2.5f), glm::vec4(0.5f, 0.3f, 0, 1));
	box5->SetOrientation(DegreeToRadian(90));
	box5->SetKinematic(true);

	Box* box6 = new Box(glm::vec2(99.f, 0), glm::vec2(0), glm::vec2(0), 10, glm::vec2(44, 2.5f), glm::vec4(0.5f, 0.3f, 0, 1));
	box6->SetOrientation(DegreeToRadian(90));
	box6->SetKinematic(true);

	Circle* box1ball1 = new Circle(glm::vec2(-20, 45.5f), glm::vec2(0), 2.0f, 2.5f, glm::vec4(0.5f, 0.3f, 0, 1));
	box1ball1->SetKinematic(true);
	Circle* box1ball2 = new Circle(glm::vec2(-90, 45.5f), glm::vec2(0), 2.0f, 2.5f, glm::vec4(0.5f, 0.3f, 0, 1));
	box1ball2->SetKinematic(true);
	
	// top right
	Circle* box2ball1 = new Circle(glm::vec2(0, 45.5f), glm::vec2(0), 2.0f, 2.5f, glm::vec4(0.5f, 0.3f, 0, 1));
	box2ball1->SetKinematic(true);
	Circle* box2ball2 = new Circle(glm::vec2(80, 45.5f), glm::vec2(0), 2.0f, 2.5f, glm::vec4(0.5f, 0.3f, 0, 1));
	box2ball2->SetKinematic(true);
	
	// bottom left
	Circle* box3ball1 = new Circle(glm::vec2(-20, -56), glm::vec2(0), 2.0f, 3, glm::vec4(0.5f, 0.3f, 0, 1));
	box3ball1->SetKinematic(true);
	Circle* box3ball2 = new Circle(glm::vec2(-90, -56), glm::vec2(0), 2.0f, 3, glm::vec4(0.5f, 0.3f, 0, 1));
	box3ball2->SetKinematic(true);


	// bottom right
	Circle* box4ball1 = new Circle(glm::vec2(0, -56), glm::vec2(0), 2.0f, 3, glm::vec4(0.5f, 0.3f, 0, 1));
	box4ball1->SetKinematic(true);
	Circle* box4ball2 = new Circle(glm::vec2(80, -56), glm::vec2(0), 2.0f, 3, glm::vec4(0.5f, 0.3f, 0, 1));
	box4ball2->SetKinematic(true);

	// left
	Circle* box5ball1 = new Circle(glm::vec2(-99.5f, -44), glm::vec2(0), 2.5f, 3, glm::vec4(0.5f, 0.3f, 0, 1));
	box5ball1->SetKinematic(true);
	Circle* box5ball2 = new Circle(glm::vec2(-99.5f, 44), glm::vec2(0), 2.5f, 3, glm::vec4(0.5f, 0.3f, 0, 1));
	box5ball2->SetKinematic(true);
	
	// right
	Circle* box6ball1 = new Circle(glm::vec2(99.5f, 44), glm::vec2(0), 2.0f, 3, glm::vec4(0.5f, 0.3f, 0, 1));
	box6ball1->SetKinematic(true);
	Circle* box6ball2 = new Circle(glm::vec2(99.5f, -44), glm::vec2(0), 2.0f, 3, glm::vec4(0.5f, 0.3f, 0, 1));
	box6ball2->SetKinematic(true);
	
	Box* trigger = new Box(glm::vec2(-55, -5), glm::vec2(0),glm::vec2(0), 10, glm::vec2(30, 40), m_triggerColor);
	trigger->SetTrigger(true);
	trigger->SetKinematic(true);
	m_trigger = trigger;
	m_physicsScene->AddActor(trigger);


	Circle* pocket1 = new Circle(glm::vec2(-10, 45), glm::vec2(0), 8.0f, 6, glm::vec4(0.2f, 0.2f, 0.2f, 1));
	pocket1->SetTrigger(true);
	pocket1->SetKinematic(true);
	m_physicsScene->AddActor(pocket1);

	Circle* pocket2 = new Circle(glm::vec2(-10, -56), glm::vec2(0), 8.0f, 6, glm::vec4(0.2f, 0.2f, 0.2f, 1));
	pocket2->SetTrigger(true);
	pocket2->SetKinematic(true);
	m_physicsScene->AddActor(pocket2);

	Circle* pocket3 = new Circle(glm::vec2(-100, -56), glm::vec2(0), 8.0f, 10, glm::vec4(0.2f, 0.2f, 0.2f, 1));
	pocket3->SetTrigger(true);
	pocket3->SetKinematic(true);
	m_physicsScene->AddActor(pocket3);

	Circle* pocket4 = new Circle(glm::vec2(-95, 45), glm::vec2(0), 8.0f, 5, glm::vec4(0.2f, 0.2f, 0.2f, 1));
	pocket4->SetTrigger(true);
	pocket4->SetKinematic(true);
	m_physicsScene->AddActor(pocket4);

	Circle* pocket5 = new Circle(glm::vec2(93, -56), glm::vec2(0), 8.0f, 10, glm::vec4(0.2f, 0.2f, 0.2f, 1));
	pocket5->SetTrigger(true);
	pocket5->SetKinematic(true);
	m_physicsScene->AddActor(pocket5);

	Circle* pocket6 = new Circle(glm::vec2(89, 45), glm::vec2(0), 8.0f, 7, glm::vec4(0.2f, 0.2f, 0.2f, 1));
	pocket6->SetTrigger(true);
	pocket6->SetKinematic(true);
	m_physicsScene->AddActor(pocket6);

	// white ball
	Circle* cue = new Circle(glm::vec2(-20, 0), glm::vec2(0), 2.0f, 3, glm::vec4(1, 1, 1, 1));
	cueBall = cue;
	

	// black ball
	Circle* blackB = new Circle(glm::vec2(20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1) ); // 4th
	blackBall = blackB;

	Circle* stripeBall1 = new Circle(glm::vec2(40, 0), glm::vec2(0), 2.0f, 4, glm::vec4(1, 1, 0, 0) ); //1st // Half
	Circle* stripeBall2 = new Circle(glm::vec2(30, 5), glm::vec2(0), 2.0f, 4, glm::vec4(1, 1, 0, 0) ); // 2nd // Half
	Circle* stripeBall3 =  new Circle(glm::vec2(40, -10), glm::vec2(0), 2.0f, 4, glm::vec4(1, 1, 0, 0) ); // 5th // Half
	Circle* stripeBall4 =  new Circle(glm::vec2(50, 5), glm::vec2(0), 2.0f, 4, glm::vec4(1, 1, 0, 0)); // 8th // Half
	Circle* stripeBall5 =  new Circle(glm::vec2(50, -15), glm::vec2(0), 2.0f, 4, glm::vec4(1, 1, 0, 0)); // 9th // Half
	Circle* stripeBall6 =  new Circle(glm::vec2(60, 20), glm::vec2(0), 2.0f, 4, glm::vec4(1, 1, 0, 0)); // 11th // Half
	Circle* stripeBall7 =  new Circle(glm::vec2(60, -20), glm::vec2(0), 2.0f, 4, glm::vec4(1, 1, 0, 0)); // 15th // Half
	
	Circle* solids1 =  new Circle(glm::vec2(30, -5), glm::vec2(0), 2.0f, 4, glm::vec4(0, 1, 0, 1) ); // 3rd // Whole
	Circle* solids2 =  new Circle(glm::vec2(40, 10), glm::vec2(0), 2.0f, 4, glm::vec4(0, 1, 0, 1)); // 6th // Whole
	Circle* solids3 =  new Circle(glm::vec2(50, -5), glm::vec2(0), 2.0f, 4, glm::vec4(0, 1, 0, 1)); // 7th // Whole
	Circle* solids4 =  new Circle(glm::vec2(50, 15), glm::vec2(0), 2.0f, 4, glm::vec4(0, 1, 0, 1)); // 10th // Whole
	Circle* solids5 =  new Circle(glm::vec2(60, -10), glm::vec2(0), 2.0f, 4, glm::vec4(0, 1, 0, 1)); // 12th // Whole
	Circle* solids6 =  new Circle(glm::vec2(60, 10), glm::vec2(0), 2.0f, 4, glm::vec4(0, 1, 0, 1)); // 13th // Whole
	Circle* solids7 =  new Circle(glm::vec2(60, 0), glm::vec2(0), 2.0f, 4, glm::vec4(0, 1, 0, 1)); // 14th // Whole
	
	m_whiteBall.push_back(cue);

	m_stripes.push_back(stripeBall1);
	m_stripes.push_back(stripeBall2);
	m_stripes.push_back(stripeBall3);
	m_stripes.push_back(stripeBall4);
	m_stripes.push_back(stripeBall5);
	m_stripes.push_back(stripeBall6);
	m_stripes.push_back(stripeBall7);
	
	m_solids.push_back(solids1);
	m_solids.push_back(solids2);
	m_solids.push_back(solids3);
	m_solids.push_back(solids4);
	m_solids.push_back(solids5);
	m_solids.push_back(solids6);
	m_solids.push_back(solids7);

	m_inHole.push_back(pocket1);
	m_inHole.push_back(pocket2);
	m_inHole.push_back(pocket3);
	m_inHole.push_back(pocket4);
	m_inHole.push_back(pocket5);
	m_inHole.push_back(pocket6);

	m_balls.push_back(stripeBall1);
	m_balls.push_back(stripeBall2);
	m_balls.push_back(stripeBall3);
	m_balls.push_back(stripeBall4);
	m_balls.push_back(stripeBall5);
	m_balls.push_back(stripeBall6);
	m_balls.push_back(stripeBall7);

	m_balls.push_back(solids1);
	m_balls.push_back(solids2);
	m_balls.push_back(solids3);
	m_balls.push_back(solids4);
	m_balls.push_back(solids5);
	m_balls.push_back(solids6);
	m_balls.push_back(solids7);

	m_physicsScene->AddActor(cue);
	m_physicsScene->AddActor(blackB);



	m_physicsScene->AddActor(solids1);
	m_physicsScene->AddActor(solids2);
	m_physicsScene->AddActor(solids3);
	m_physicsScene->AddActor(solids4);
	m_physicsScene->AddActor(solids5);
	m_physicsScene->AddActor(solids6);
	m_physicsScene->AddActor(solids7);

	m_physicsScene->AddActor(stripeBall1);
	m_physicsScene->AddActor(stripeBall2);
	m_physicsScene->AddActor(stripeBall3);
	m_physicsScene->AddActor(stripeBall4);
	m_physicsScene->AddActor(stripeBall5);
	m_physicsScene->AddActor(stripeBall6);
	m_physicsScene->AddActor(stripeBall7);

	// top 2 boxes
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);
	
	// bottom 2 boxes
	m_physicsScene->AddActor(box3);
	m_physicsScene->AddActor(box4);
	
	//left box
	m_physicsScene->AddActor(box5);
	//right box
	m_physicsScene->AddActor(box6);

	m_physicsScene->AddActor(pocket1);
	m_physicsScene->AddActor(pocket2);
	m_physicsScene->AddActor(pocket3);
	m_physicsScene->AddActor(pocket4);
	m_physicsScene->AddActor(pocket5);
	m_physicsScene->AddActor(pocket6);
	
	
	m_physicsScene->AddActor(box1ball1);
	m_physicsScene->AddActor(box1ball2);

	m_physicsScene->AddActor(box2ball1);
	m_physicsScene->AddActor(box2ball2);
	
	m_physicsScene->AddActor(box3ball1);
	m_physicsScene->AddActor(box3ball2);
	
	m_physicsScene->AddActor(box4ball1);
	m_physicsScene->AddActor(box4ball2);
	
	m_physicsScene->AddActor(box5ball1);
	m_physicsScene->AddActor(box5ball2);
	
	m_physicsScene->AddActor(box6ball1);
	m_physicsScene->AddActor(box6ball2);

	for (int i = 0; i < m_stripes.size(); i++)
	{
		m_stripes[i]->SetStripe(true);
	}

	
#endif // PoolTable

#ifdef ContactForces
	m_physicsScene->SetGravity(glm::vec2(0, -32.f));
	

	//Box* box1 = new Box(glm::vec2(0, -30), glm::vec2(0), 5.0f, glm::vec2(50, 10), glm::vec4(0, 1, 1, 1));

	Box* staticBox = new Box(glm::vec2(0, -50), glm::vec2(0), glm::vec2(0), 5.0f, glm::vec2(2.5f, 100), glm::vec4(0, 1, 1, 1));
	staticBox->SetOrientation(DegreeToRadian(90));

	
	Circle* ball1 = new Circle(glm::vec2(-20, 10), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 10), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(staticBox);
	
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);


#endif // ContactForces

#ifdef Beads
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Circle* prev = nullptr;
	for (int i = 0; i < num; i++)
	{
		// spawn a circle to the right and below the previous one, so that the whole rope acts under gravity and swings
		Circle* circle = new Circle(glm::vec2(i * 3, 30 - i * 5), glm::vec2(0), 10, 2, glm::vec4(1, 0, 0, 1));
		if (i == 0)
			circle->SetKinematic(true);
		m_physicsScene->AddActor(circle);
		if (prev)
			m_physicsScene->AddActor(new Spring(circle, prev, 500, 10, 7));
		prev = circle;
	}

	// add a kinematic box at an angle for the rope to drape over
	//Box* box = new Box(glm::vec2(0, -20), glm::vec2(0), 0.3f, 20, 8, 2, glm::vec4(0, 0, 1, 1));
	Box* box = new Box(glm::vec2(0, -50), glm::vec2(0), glm::vec2(0), 5.0f, glm::vec2(2.5f, 100), glm::vec4(0, 1, 1, 1));
	
	box->SetKinematic(true);
	m_physicsScene->AddActor(box);


#endif // Beads

#ifdef TriggerTest
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Circle* ball1 = new Circle(glm::vec2(20, 20), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, -20), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	ball2->SetKinematic(true);
	ball2->SetTrigger(true);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -30));
	m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -50));
	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -50));
	m_physicsScene->AddActor(new Box(glm::vec2(20, 10), glm::vec2(0), glm::vec2(0), 5.0f, glm::vec2(44, 2.5f), glm::vec4(0, 1, 1, 1)));
	m_physicsScene->AddActor(new Box(glm::vec2(-40, 10), glm::vec2(0), glm::vec2(0), 5.0f, glm::vec2(44, 2.5f), glm::vec4(0, 1, 1, 1)));

	ball1->collisionCallback = [=](PhysicsObject* other) {
		if (other == ball2)
		{
			std::cout << "Howzat!!?" << std::endl;
		}
		return;
	};

	ball2->triggerEnter = [=](PhysicsObject* other) { std::cout << "Enter:" << other << std::endl; };
	ball2->triggerExit = [=](PhysicsObject* other) { std::cout << "Exit:" << other << std::endl; };

#endif // TriggerTest


}

void PhysicsApp::SpawnBalls()
{

}

void PhysicsApp::HasBeenSunk()
{
	cueBall->SetVelocity(glm::vec2(0.001f, 0.001f));
	cueBall->SetPosition(glm::vec2(-20, 0));
}

void PhysicsApp::DemoUpdates(aie::Input* input, float dt)
{

}

float PhysicsApp::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}
