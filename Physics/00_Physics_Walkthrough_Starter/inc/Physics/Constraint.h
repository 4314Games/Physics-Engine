#pragma once

#include <vector>

namespace Physics
{
	class PhysicsObject;

	class Constraint
	{
	public:
		
		enum class Type
		{
			SPRING, JOINT
		};

		Constraint(std::vector<PhysicsObject*> & objects, Type type);
		virtual ~Constraint();

		virtual void Update(float deltaTime) = 0;

		const std::vector<PhysicsObject*> GetObjects() { return m_objects; }
		Type GetType() const { return m_type; }

	protected:
		std::vector<PhysicsObject*> m_objects;
		Type m_type;

	private:
	};
}
