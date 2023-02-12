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
	m_physicsScene->SetGravity(glm::vec2(0,-9.82f));
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

	if (input->isMouseButtonDown(0) && cueBall->GetVelocity() == glm::vec2(0))
	{
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		glm::vec2 whitePos = cueBall->GetPosition();
		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));

		aie::Gizmos::add2DLine(whitePos, whitePos - ScreenToWorld(mousePos), glm::vec4(1, 1, 1, 1));
	}

	if (input->wasMouseButtonReleased(0) && cueBall->GetVelocity() == glm::vec2(0))
	{
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		glm::vec2 whitePos = cueBall->GetPosition();
		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));

		cueBall->ApplyForce(mouseToWhite * glm::vec2(800), glm::vec2(0));

		m_playersTurn == 1 ? m_playersTurn = 2 : m_playersTurn = 1;
	}

	
}

void PhysicsApp::draw() 
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
	Plane* plane4 = new Plane(glm::vec2(0, -1), -50); // top
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50); // bottom
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -90); // right
	Plane* plane3 = new Plane(glm::vec2(-1, 0), 100); // left

	// cue ball
	Circle* cue = new Circle(glm::vec2(-20, 0), glm::vec2(0), 2.0f, 3, glm::vec4(1, 0, 0, 1));
	cueBall = cue;

	Circle* ball1 = new Circle(glm::vec2(20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(30, 5), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(30, -5), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball4 = new Circle(glm::vec2(40, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	
	Circle* ball5 = new Circle(glm::vec2(40, -10), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball6 = new Circle(glm::vec2(40, 10), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball7 = new Circle(glm::vec2(50, -5), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball8 = new Circle(glm::vec2(50, 5), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	
	Circle* ball9 = new Circle(glm::vec2(50, -15), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball10 = new Circle(glm::vec2(50, 15), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	
	Circle* ball11 = new Circle(glm::vec2(60, 20), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball12 = new Circle(glm::vec2(60, -10), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	
	Circle* ball13 = new Circle(glm::vec2(60, 10), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball14 = new Circle(glm::vec2(60, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball15 = new Circle(glm::vec2(60, -20), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	



	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	m_physicsScene->AddActor(plane4);

	m_physicsScene->AddActor(cue);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);
	m_physicsScene->AddActor(ball6);
	m_physicsScene->AddActor(ball7);
	m_physicsScene->AddActor(ball8);
	m_physicsScene->AddActor(ball9);
	m_physicsScene->AddActor(ball10);
	m_physicsScene->AddActor(ball11);
	m_physicsScene->AddActor(ball12);
	m_physicsScene->AddActor(ball13);
	m_physicsScene->AddActor(ball14);
	m_physicsScene->AddActor(ball15);
	

	
#endif // PoolTable
#ifdef ContactForces
	m_physicsScene->SetGravity(glm::vec2(0, -32.f));
	

	Box* box1 = new Box(glm::vec2(0, -30), glm::vec2(0), 5.0f, glm::vec2(50, 10), glm::vec4(0, 1, 1, 1));

	Circle* ball1 = new Circle(glm::vec2(-20, 10), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 10), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(box1);
	
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
	Box* box = new Box(glm::vec2(0, -20), glm::vec2(0), 0.3f, 20, 8, 2, glm::vec4(0, 0, 1, 1));
	
	box->SetKinematic(true);
	m_physicsScene->AddActor(box);


#endif // Beads

}

void PhysicsApp::DemoUpdates(aie::Input* input, float dt)
{

}

float PhysicsApp::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}
