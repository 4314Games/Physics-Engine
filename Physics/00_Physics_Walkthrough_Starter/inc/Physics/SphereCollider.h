#pragma once
#include "Collider.h"
#include <glm\vec3.hpp>
#include "PhysicsObject.h"

namespace Physics
{
	class PhysicsObject;

	class SphereCollider : public Collider
	{
	public:
		SphereCollider();
		SphereCollider(float p_radius);
		virtual ~SphereCollider();
	
		const glm::vec3 & GetPos() const { return m_position; }
		float GetRadius() const { return m_radius; }

		void SetPosition(const glm::vec3 & p_position) { m_position = p_position; }
		void SetRadius(float p_radius) { m_radius = p_radius; }

		bool Intersects(Collider* other) const;

		virtual void Transform(PhysicsObject *p_obj);

	protected:

		glm::vec3 m_position;
		float m_radius;

	private:
	};
}
