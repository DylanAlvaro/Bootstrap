#pragma once
#include <glm/vec2.hpp>
#include <vector>

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	void RemoveActor(PhysicsObject* actor);
	void Update(float dt);
	void Draw();
	void DebugScene();

	//Getters
	glm::vec2 GetGravity() { return m_gravity; }
	float GetTimeStep() { return m_timeStep; }

	//Setters
	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }

	const int SHAPE_COUNT = 3;

	void CheckForCollision();

	static bool Circle2Circle(PhysicsObject*, PhysicsObject*);
	static bool Plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool Circle2Plane(PhysicsObject*, PhysicsObject*);
	static bool Plane2Circle(PhysicsObject*, PhysicsObject*);

private:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};

