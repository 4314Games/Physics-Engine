#include "Physics\PhysicsScene.h"
#include "Physics\PhysicsObject.h"
#include "Physics\Collider.h"

#include <algorithm>

#include <glm\geometric.hpp>

using namespace Physics;

PhysicsScene::PhysicsScene()
{
}


PhysicsScene::~PhysicsScene()
{
	for (auto iter = m_objects.begin(); iter != m_objects.end(); iter++)
	{
		delete *iter;
	}
	m_objects.clear();
}

void Physics::PhysicsScene::Update(float deltaTime)
{
	for (auto iter = m_objects.begin(); iter != m_objects.end(); iter++)
	{
		PhysicsObject *obj = *iter;
		obj->SetAcceleration(gravAccel);
		obj->ApplyForce(m_globalForce);
		obj->Update(deltaTime);

		// Super dodgy ground collision bouncing
		// This will be replaced by proper collision detection and reaction later
		//-------------------------------------------------------------
		const glm::vec3 & pos = obj->GetPosition();
		const glm::vec3 & vel = obj->GetVelocity();

		if (pos.y < 0.0f)
		{
			obj->SetPosition(glm::vec3(pos.x, 0.0f, pos.z));
			obj->SetVelocity(glm::vec3(vel.x, -vel.y, vel.z));
		}

		//-------------------------------------------------------------

	}
	m_globalForce = glm::vec3(); // option to turn off gravity each frame
									// which would mean you'd have to apply it
									// every frame
	DetectCollisions();
	ResolveCollisions();
}

void Physics::PhysicsScene::ApplyGlobalForce(const glm::vec3 & force)
{
	m_globalForce = force;
}

void Physics::PhysicsScene::AttachObject(PhysicsObject * obj)
{
	auto iter = std::find(m_objects.begin(), m_objects.end(), obj);
	if (iter == m_objects.end()) // object pointer is not already in our vector
	{
		m_objects.push_back(obj);
	}
}

void Physics::PhysicsScene::RemoveObject(PhysicsObject * obj)
{
	auto iter = std::find(m_objects.begin(), m_objects.end(), obj);
	if (iter != m_objects.end()) // object pointer is in our vector
	{
		delete *iter; // maybe this is needed, but maybe we just "give" the object back to another class
		m_objects.erase(iter);
	}
}

const std::vector<PhysicsObject*>& Physics::PhysicsScene::GetObjects() const
{
	return m_objects;
}

bool Physics::PhysicsScene::isObjectColliding(PhysicsObject * obj)
{
	// MAP

	// super inefficient searching . . . going to need to change this
	for (auto iter = m_collisions.begin(); iter != m_collisions.end(); iter++)
	{
		if ((*iter).objA == obj || (*iter).objB == obj)
			return true;
	}
	return false;
}

void Physics::PhysicsScene::DetectCollisions()
{
	m_collisions.clear(); // remove old collisions from previous frame

	// Super inefficient brute force collision detection
	for (auto iterA = m_objects.begin(); iterA != m_objects.end(); iterA++)
	{
		PhysicsObject* objA = *iterA;

		// iterB = iterA + 1 stops double checking
		// also stops it from checking itself
		for (auto iterB = iterA + 1; iterB != m_objects.end(); iterB++)
		{
			PhysicsObject* objB = *iterB;

			CollisionInfo info;
			// do the colliders of the two objects overlap?
			if (objA->GetCollider()->Intersects(objB->GetCollider(), &info.intersect))
			{	
				info.objA = objA;
				info.objB = objB;
				m_collisions.push_back(info);
			}
		}
	}
}

void Physics::PhysicsScene::ResolveCollisions()
{
	// Loop through all collision pairs
	for (auto iter = m_collisions.begin(); iter != m_collisions.end(); iter++)
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
		iter->objA->SetVelocity(velA - impulse / massA);
		iter->objB->SetVelocity(velB + impulse / massB);

		// move the spheres so that they're not overlapping
		glm::vec3 separate = iter->intersect.collisionVector * 0.5f;
		iter->objA->SetPosition(iter->objA->GetPosition() - separate);
		iter->objB->SetPosition(iter->objB->GetPosition() + separate);
	}
}
