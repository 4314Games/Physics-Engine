#pragma once
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Collider.h"

namespace Physics
{
	class Collider;

	class PhysicsObject
	{
	public:
		PhysicsObject();
		~PhysicsObject();

		void Update(float deltaTime);
		
		void ApplyForce(const glm::vec3 p_force);

		const glm::vec3 GetPos() const { return m_position; }
		const glm::vec3 GetVelocity() const { return m_velocity; }
		const glm::vec3 GetAcceleration() const { return m_acceleration; }
		const float GetMass() const	{ return m_mass; }
		const float GetFriction() const	{ return m_friction; }
		Collider* GetCollider();

		void SetPos(const glm::vec3 &p_pos) { m_position = p_pos; }
		void SetVelocty(const glm::vec3 &p_velocity) { m_velocity = p_velocity; }
		void SetAcceleration(const glm::vec3 &p_accel) { m_acceleration = p_accel; }
		void SetMass(float p_mass) { m_mass = p_mass;	}
		void SetFriction(float p_friction) { m_friction = p_friction; }
		void SetCollider(Collider* p_collider);

	protected:
		glm::vec3 m_position;
		glm::vec3 m_velocity;
		glm::vec3 m_acceleration;
		float m_mass = 1.0f;
		float m_friction = 1.0f;
	
		Collider *m_collider = nullptr;
	};
}
