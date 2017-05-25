#pragma once
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "Intersect.h"

namespace Physics
{
	class PhysicsObject;
	class Constraint;

	class PhysicScene
	{
	public:

		struct CollisionInfo
		{
			PhysicsObject* objA;
			PhysicsObject* objB;

			IntersectData intersect;
		};

		PhysicScene();
		virtual ~PhysicScene();
	
		void Update(float deltaTime);

		//Apply force to all objects
		void ApplyGlobalForce(const glm::vec3 & force);

		//attach a physics obect to our scene
		void AttachObject(PhysicsObject * p_obj);

		//remove a physics object from our ecene
		void RemoveObject(PhysicsObject * p_obj);

		const std::vector<PhysicsObject*> & GetObjects() const;

		bool isObjectColliding(PhysicsObject* obj);

		void AttachConstraint(Constraint * con);
		void RemoveConstraint(Constraint * con);
		const std::vector<Constraint *> & GetConstraints() const;

	protected:
		std::vector<PhysicsObject *> m_object;
		std::vector<Constraint *> m_constraints;
		glm::vec3 m_globalForce;

		void DetectCollision();
		void ResolveCollision();
		std::vector<CollisionInfo> m_collision;

		std::map<PhysicsObject*, bool> m_isColidingLookUp;
		glm::vec3 gravAccel = glm::vec3(0.0f, -9.8f, 0.0f);

	private:
		
	};
}
