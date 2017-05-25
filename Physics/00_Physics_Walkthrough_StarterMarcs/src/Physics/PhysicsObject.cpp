#include "Physics\PhysicsObject.h"
#include "Physics\Collider.h"

using namespace Physics;

PhysicsObject::PhysicsObject()
{
}


PhysicsObject::~PhysicsObject()
{
	delete m_collider;
}

// Force = mass * acceleration
// Acceleration = Force / mass

// acceleration changes velocity over time
// velocity changes position over time

void PhysicsObject::Update(float deltaTime)
{
	ApplyForce(-m_velocity * m_friction);
	m_velocity += m_acceleration * deltaTime;
	m_position += m_velocity * deltaTime;
	m_acceleration = glm::vec3();
	m_collider->Transform(this);
}

void PhysicsObject::ApplyForce(const glm::vec3 & force)
{
	m_acceleration += force / m_mass;
}

Collider * PhysicsObject::GetCollider()
{
	if (m_collider == nullptr)
	{
		return Collider::GetNullInstance();
	}
	return m_collider;
}

void PhysicsObject::SetCollider(Collider * collider)
{
	// clear memory for the old collider we owned (if we did own one)
	delete m_collider;

	// take ownership of a new collider
	m_collider = collider;
	m_collider->Transform(this);
}
