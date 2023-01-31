#include "PhysicsScene.h"
#include "PhysicsObject.h"

#include <list>
#include <vector>
#include "Circle.h"

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = glm::vec2(0);
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
		delete pActor;
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		m_actors.erase(m_actors.begin() + i);
	}
}

bool PhysicsScene::Circle2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Circle* sphere1 = dynamic_cast<Circle*>(obj1);
	Circle* sphere2 = dynamic_cast<Circle*>(obj2);

	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		// TODO do the necessary maths in here
		// TODO if the Circles touch, set their velocities to zero for now

		return(glm::distance(sphere1->GetPosition(), sphere2->GetPosition()) < sphere1->GetRadius() +  sphere2->GetRadius());
	}
	return true;
}


void PhysicsScene::Update(float dt)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep) {
		for (auto pActor : m_actors) {
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

		int actorCount = m_actors.size();

		for (int outer = 0; outer < actorCount - 1; outer++)
		{
			for (int inner = outer + 1; inner < actorCount; inner++)
			{
				PhysicsObject* object1 = m_actors[outer];
				PhysicsObject* object2 = m_actors[inner];
				
				if (Circle2Circle(object1, object2))
				{
					dynamic_cast<Circle*>(object1)->ApplyForce(dynamic_cast<Circle*>(object1)->GetVelocity() * -dynamic_cast<Circle*>(object2)->GetMass());
					dynamic_cast<Circle*>(object2)->ApplyForce(dynamic_cast<Circle*>(object2)->GetVelocity() * -dynamic_cast<Circle*>(object1)->GetMass());
				}
			}
		}
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw(1);
	}
	
}

