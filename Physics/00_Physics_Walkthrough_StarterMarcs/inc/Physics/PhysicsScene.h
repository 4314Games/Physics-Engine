#pragma once

#include "Intersect.h"

#include <vector>

#include <glm\vec3.hpp>

namespace Physics
{
	class PhysicsObject;

	class PhysicsScene
	{
	public:

		struct CollisionInfo
		{
			PhysicsObject* objA;
			PhysicsObject* objB;

			IntersectData intersect;
		};

		PhysicsScene();
		virtual ~PhysicsScene();

		void Update(float deltaTime);

		// Apply force to all objects
		void ApplyGlobalForce(const glm::vec3 & force);

		// attach a Physics Object to the scene
		void AttachObject(PhysicsObject* obj);

		// remove a Physics Object from the scene
		void RemoveObject(PhysicsObject* obj);

		const std::vector<PhysicsObject*> & GetObjects() const;

		bool isObjectColliding(PhysicsObject* obj);

	protected:
		std::vector<PhysicsObject *> m_objects;
		glm::vec3 m_globalForce;

		void DetectCollisions();
		void ResolveCollisions();
		std::vector<CollisionInfo> m_collisions;
		//std::map<PhysicsObject *, bool> m_isCollidingLookup; // speeds up lookup for whether collisions have happened
															 // but it's totally not what we would do
		glm::vec3 gravAccel = glm::vec3(0.0f, -9.8f, 0.0f);
	private:
	};
}

