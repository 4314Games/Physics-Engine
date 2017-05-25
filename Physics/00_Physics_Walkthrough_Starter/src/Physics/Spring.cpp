#include "..\inc\Physics\Spring.h"
#include "Physics\PhysicsObject.h"
#include <glm\/vec3.hpp>
#include <glm/geometric.hpp>

using namespace Physics;

//Constructors make springs using exactly two objects

Physics::Spring::Spring(PhysicsObject * objA, PhysicsObject * objB):
	Constraint(std::vector < PhysicsObject*>{ objA, objB }, Constraint::Type::SPRING),
	m_length(5.0f),
	m_stiffness(100.0f),
	m_friction(1.0f)
{
	
}

Physics::Spring::Spring(PhysicsObject * objA, PhysicsObject * objB, float length, float stiffness, float friction) :
	Constraint(std::vector < PhysicsObject*>{ objA, objB }, Constraint::Type::SPRING),
	m_friction(friction),
	m_length(length),
	m_stiffness(stiffness)
{
}

Spring::~Spring()
{
}

void Physics::Spring::Update(float deltaTime)
{
	PhysicsObject *objA = m_objects[0];
	PhysicsObject *objB = m_objects[1];

	//measure the distance between the two objects
	glm::vec3 springVec = objB->GetPos() - objA->GetPos();
	float distance = glm::length(springVec);
	//create a force along the vector netweem tje,. based oon how far we are from our length
	glm::vec3 force;
	if (distance != 0)
	{
		force += glm::normalize(springVec) * (distance - m_length) * m_stiffness;
		force += (glm::dot(objB->GetVelocity() - objA->GetVelocity(), springVec)) * glm::normalize(springVec) * m_friction;
	}
	//edit that force for feictions(the faster thigns are moving the more we slow them down)

	//apply force to bopthe objects in the required direction
	objA->ApplyForce(force);
	objB->ApplyForce(-force);
}