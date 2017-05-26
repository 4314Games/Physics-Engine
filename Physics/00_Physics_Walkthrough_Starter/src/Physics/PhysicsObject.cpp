#include "Physics\PhysicsObject.h"
#include "Physics\Collider.h"

namespace Physics
{
	PhysicsObject::PhysicsObject()
	{
	}


	PhysicsObject::~PhysicsObject()
	{
		delete m_collider;
	}

	void PhysicsObject::Update(float deltaTime)		//Updates position, velocity and acceleration
	{
		ApplyForce(-m_velocity * m_friction);
		m_velocity += m_acceleration * deltaTime;
		m_position += m_velocity * deltaTime;
		m_acceleration = glm::vec3();
		m_collider->Transform(this);
	}

	void PhysicsObject::ApplyForce(glm::vec3 p_force)		//Apply a force 
	{
		m_acceleration += p_force / m_mass;		
	}

	Collider * PhysicsObject::GetCollider()
	{
		if (m_collider == nullptr)
		{
			return Collider::GetNullInstance();
		}
		return m_collider;
	}

	void PhysicsObject::SetCollider(Collider * p_collider)		//Apply a collider type
	{
		//Clear memory for the old collider we owned
		delete m_collider;

		//Take ownership pf a mew collider
		m_collider = p_collider;
		m_collider->Transform(this);
	}
}