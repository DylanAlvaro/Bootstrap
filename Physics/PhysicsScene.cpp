#include "PhysicsScene.h"
#include "PhysicsObject.h"

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = glm::vec2(0);
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
}

void PhysicsScene::Update(float dt)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw(1);
	}
}
