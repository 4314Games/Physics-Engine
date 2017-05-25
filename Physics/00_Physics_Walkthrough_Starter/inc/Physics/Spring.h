#pragma once
#include "Constraint.h"

namespace Physics
{
	class Spring :
		public Constraint
	{
	public:
		Spring(PhysicsObject *objA, PhysicsObject *objB);
		Spring(PhysicsObject *objA, PhysicsObject *objB, float length, float stiffness, float friction);
		virtual ~Spring();

		void Update(float deltaTime);

	protected:
		float m_length;
		float m_stiffness;
		float m_friction;

	private:
	};
}