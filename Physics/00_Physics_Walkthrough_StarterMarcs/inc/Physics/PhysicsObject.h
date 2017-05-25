#pragma once

#include <glm\vec3.hpp>

namespace Physics
{
	class Collider;

	class PhysicsObject
	{
	public:
		PhysicsObject();
		virtual ~PhysicsObject();

		void Update(float deltaTime);

		void ApplyForce(const glm::vec3 & force);

		// Getters and Setters
		const glm::vec3 & GetPosition() const			{ return m_position; }
		const glm::vec3 & GetVelocity() const			{ return m_velocity; }
		const glm::vec3 & GetAcceleration() const		{ return m_acceleration; }
		const float GetMass() const					{ return m_mass; }
		const float GetFriction() const				{ return m_friction; }
		Collider* GetCollider();
		const float GetBounciness() const			{ return m_bounciness; }

		void SetPosition(const glm::vec3 & pos)		{ m_position = pos; }
		void SetVelocity(const glm::vec3 & vel)		{ m_velocity = vel; }
		void SetAcceleration(const glm::vec3 & acc) { m_acceleration = acc; }
		void SetMass(float mass)					{ m_mass = mass; }
		void SetFriction(float friction)			{ m_friction = friction; }
		void SetCollider(Collider* collider);
		void SetBounciness(float b)					{ m_bounciness = b; }

	protected:
		glm::vec3 m_position;
		glm::vec3 m_velocity;
		glm::vec3 m_acceleration;
		float m_mass = 1.0f;
		float m_friction = 1.0f;
		float m_bounciness = 1.0f;

		Collider *m_collider = nullptr;

	};

}

