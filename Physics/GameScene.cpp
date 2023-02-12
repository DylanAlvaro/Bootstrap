#include "GameScene.h"
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

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::startup()
{
    aie::Gizmos::create(255U, 255U, 65535U, 65535U);


	m_2dRenderer = new aie::Renderer2D();


	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));
	m_physicsScene->SetTimeStep(0.01f);

	return true;
}

void GameScene::shutdown()
{
}

void GameScene::update(float deltaTime)
{
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();


	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (input->isMouseButtonDown(0) && cueBallW->GetVelocity() == glm::vec2(0))
	{
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		glm::vec2 whitePos = cueBallW->GetPosition();
		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));

		aie::Gizmos::add2DLine(whitePos, whitePos - ScreenToWorld(mousePos), glm::vec4(1, 1, 1, 1));
	}

	if (input->wasMouseButtonReleased(0) && cueBallW->GetVelocity() == glm::vec2(0))
	{
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		glm::vec2 whitePos = cueBallW->GetPosition();
		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));

		cueBallW->ApplyForce(mouseToWhite * glm::vec2(800, 800), glm::vec2(0));

		m_playersTurn == 1 ? m_playersTurn = 2 : m_playersTurn = 1;
	}
}

void GameScene::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();



	// draw your stuff here!
	static float aspectRatio = 16.f / 9.f;

	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));

	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void GameScene::SetBallPositions()
{
}

void GameScene::AssignBallType()
{
}

glm::vec2 GameScene::ScreenToWorld(glm::vec2 screenPos)
{
    return glm::vec2();
}

float GameScene::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}
