#include "..\..\inc\Physics\PhysicScene.h"
#include "Physics\PhysicsObject.h"
#include <algorithm>
#include "Physics\Collider.h"
#include  <glm\geometric.hpp>
#include "Physics\Constraint.h"

namespace Physics
{

	PhysicScene::PhysicScene()
	{
	}


	PhysicScene::~PhysicScene()
	{
		for (auto iter = m_object.begin(); iter != m_object.end(); iter++)
		{
			delete *iter;
		}
		m_object.clear();
		for (auto iter = m_constraints.begin(); iter != m_constraints.end(); iter++)
		{
			delete *iter;
		}
		m_constraints.clear();
	}

	void Physics::PhysicScene::Update(float deltaTime)
	{
		//update for constraints
		for (auto iter = m_constraints.begin(); iter != m_constraints.end(); iter++)
		{
			(*iter)->Update(deltaTime);
		}

		//update all objects
		for (auto iter = m_object.begin(); iter != m_object.end(); iter++)
		{
			PhysicsObject *obj = *iter;
			//glm::vec3 currentAccel = obj->GetAcceleration();
			//obj->SetAcceleration(gravAccel + currentAccel);
			obj->ApplyForce(m_globalForce);
			obj->Update(deltaTime);

			// Super dodgy ground collision bouncing
			// This will be replaced by proper collision detection and reaction later
			//-------------------------------------------------------------
			const glm::vec3 & pos = obj->GetPos();
			const glm::vec3 & vel = obj->GetVelocity();

			if (pos.y < 0.0f)
			{
				obj->SetPos(glm::vec3(pos.x, 0.0f, pos.z));
				obj->SetVelocty(glm::vec3(vel.x, -vel.y, vel.z));
			}

			//-------------------------------------------------------------

		}
		m_globalForce = glm::vec3(); // option to turn off gravity each frame
									 // which would mean you'd have to apply it
									 // every frame
		DetectCollision();
		ResolveCollision();
	}

	void Physics::PhysicScene::ApplyGlobalForce(const glm::vec3 & force)
	{
		m_globalForce = force;
	}

	void Physics::PhysicScene::AttachObject(PhysicsObject * obj)
	{
		auto iter = std::find(m_object.begin(), m_object.end(), obj);
		if (iter == m_object.end()) // object pointer is not already in our vector
		{
			m_object.push_back(obj);
		}
	}

	void Physics::PhysicScene::RemoveObject(PhysicsObject * obj)
	{
		auto iter = std::find(m_object.begin(), m_object.end(), obj);
		if (iter != m_object.end()) // object pointer is in our vector
		{
			delete *iter; // maybe this is needed, but maybe we just "give" the object back to another class
			m_object.erase(iter);
		}
	}

	const std::vector<PhysicsObject*>& Physics::PhysicScene::GetObjects() const
	{
		return m_object;
	}

	bool Physics::PhysicScene::isObjectColliding(PhysicsObject * obj)
	{
		// MAP

		// super inefficient searching . . . going to need to change this
		for (auto iter = m_collision.begin(); iter != m_collision.end(); iter++)
		{
			if ((*iter).objA == obj || (*iter).objB == obj)
				return true;
		}
		return false;
	}

	void PhysicScene::AttachConstraint(Constraint * con)
	{
		auto iter = std::find(m_constraints.begin(), m_constraints.end(), con);
		if (iter == m_constraints.end())
		{
			m_constraints.push_back(con);		
		}

		
	}

	void PhysicScene::RemoveConstraint(Constraint * con)
	{
		auto iter = std::find(m_constraints.begin(), m_constraints.end(), con);
		if (iter != m_constraints.end())
		{
			delete *iter;
			m_constraints.erase(iter);
		}
	}

	const std::vector<Constraint*>& PhysicScene::GetConstraints() const
	{
		// TODO: insert return statement here
		return m_constraints;
	}

	void Physics::PhysicScene::DetectCollision()
	{
		m_collision.clear(); // remove old collisions from previous frame

							  // Super inefficient brute force collision detection
		for (auto iterA = m_object.begin(); iterA != m_object.end(); iterA++)
		{
			PhysicsObject* objA = *iterA;

			// iterB = iterA + 1 stops double checking
			// also stops it from checking itself
			for (auto iterB = iterA + 1; iterB != m_object.end(); iterB++)
			{
				PhysicsObject* objB = *iterB;

				CollisionInfo info;
				// do the colliders of the two objects overlap?
				if (objA->GetCollider()->Intersects(objB->GetCollider(), &info.intersect))
				{
					info.objA = objA;
					info.objB = objB;
					m_collision.push_back(info);
				}
			}
		}
	}

	void Physics::PhysicScene::ResolveCollision()
	{
		// Loop through all collision pairs
		for (auto iter = m_collision.begin(); iter != m_collision.end(); iter++)
		{
			// Get data from the collision

			// collision normal (direction of collision and overlap)
			glm::vec3 colNormal = iter->intersect.collisionVector;

			// mass of both objects
			float massA = iter->objA->GetMass();
			float massB = iter->objB->GetMass();

			// velocity of both objects
			glm::vec3 velA = iter->objA->GetVelocity();
			glm::vec3 velB = iter->objB->GetVelocity();
			// and relative velocity
			glm::vec3 relVel = velA - velB;

			// find out how much velocity each object had in the collision normal direction
			// In fact, since we have the relative velocity, we can just find out once
			// how much total velocity there is in the collision normal direction
			glm::vec3 colVector = colNormal * (glm::dot(relVel, colNormal));

			// Find the bounciness of the collision
			float bounciness = glm::min(iter->objA->GetBounciness(), iter->objB->GetBounciness());

			// calculate the impulse force (vector of force and direction)
			glm::vec3 impulse = (1.0f + bounciness) * colVector / (1.0f / massA + 1.0f / massB);


			// apply that force to both objects (each one in an opposite direction)
			iter->objA->SetVelocty(velA - impulse / massA);
			iter->objB->SetVelocty(velB + impulse / massB);

			// move the spheres so that they're not overlapping
			glm::vec3 separate = iter->intersect.collisionVector * 0.5f;
			iter->objA->SetPos(iter->objA->GetPos() - separate);
			iter->objB->SetPos(iter->objB->GetPos() + separate);
		}
	}
}