#include "PhysicsScene.h"
#include "PhysicsObject.h"

#include <list>
#include <vector>
#include "Circle.h"
#include "Input.h"
#include "Plane.h"

glm::vec2 PhysicsScene::m_gravity = glm::vec2(0);

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
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

void PhysicsScene::CheckForCollision()
{
}

bool PhysicsScene::Circle2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Circle* circle1 = dynamic_cast<Circle*>(obj1);
	Circle* circle2 = dynamic_cast<Circle*>(obj2);
	if (circle1 != nullptr && circle2 != nullptr) 
	{
		float dist = distance(circle1->GetPosition(), circle2->GetPosition());
		if (glm::length(dist) < circle1->GetRadius() + circle2->GetRadius()) 
		{
			circle1->ResolveCollision(circle2, 0.5f * (circle1->GetPosition() + circle2->GetPosition()));
			return true;
			}
	}
	return false;
}

bool PhysicsScene::Plane2Plane(PhysicsObject*, PhysicsObject*)
{
	//no math required just return false ( i think ) 
	return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Circle* circle = dynamic_cast<Circle*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	if (circle != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		glm::vec2 contact = circle->GetPosition() + (collisionNormal * -circle->GetRadius());
		float sphereToPlane = glm::dot(circle->GetPosition(), plane->GetNormal()) - plane->GetDistance();

		float intersection = circle->GetRadius() - sphereToPlane;
		float velocityOutOfPlane = glm::dot(circle->GetVelocity(), plane->GetNormal());

		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			plane->ResolveCollision(circle, contact);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Circle2Plane(obj2, obj1);
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Circle,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle,
};


void PhysicsScene::Update(float dt)
{
	aie::Input* input = aie::Input::getInstance();


	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep) 
	{
		for (auto pActor : m_actors) 
		{
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
				int shapeId1 = object1->GetShapeID();
				int shapeId2 = object2->GetShapeID();

				int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;

				fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
				
				if (collisionFunctionPtr != nullptr)
				{
					collisionFunctionPtr(object1, object2);
				}

				//if (Circle2Circle(object1, object2))
				//{
				//	dynamic_cast<Circle*>(object1)->ApplyForce(dynamic_cast<Circle*>(object1)->GetVelocity()); // get rid of everything after the * for newtons third to work
				//	dynamic_cast<Circle*>(object2)->ApplyForce(dynamic_cast<Circle*>(object2)->GetVelocity()); // same here
				//}
				//
				//if (input->isKeyDown(aie::INPUT_KEY_D))
				//{
				//	dynamic_cast<Circle*>(object1)->ApplyForce(glm::vec2(30, 0));
				//	dynamic_cast<Circle*>(object2)->ApplyForce(glm::vec2(-30, 0));
				//}
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

