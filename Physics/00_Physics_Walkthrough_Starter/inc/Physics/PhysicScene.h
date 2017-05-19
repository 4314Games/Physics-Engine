#pragma once
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

namespace Physics
{
	class PhysicsObject;

	class PhysicScene
	{
	public:

		struct CollisionInfo
		{
			PhysicsObject* objA;
			PhysicsObject* objB;


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

	protected:
		std::vector<PhysicsObject *> m_object;
		glm::vec3 m_globalForce;

		void DetectCollision();
		std::vector<CollisionInfo> m_collision;
		std::map<PhysicsObject*, bool> m_isColidingLookUp;

	private:
		
	};
}
