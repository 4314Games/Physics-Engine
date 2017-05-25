#include "..\..\inc\Physics\Collider.h"
#include "Physics\SphereCollider.h"
#include <glm\vec3.hpp>
#include <glm\geometric.hpp>
#include "Physics\PhysicScene.h"

namespace Physics
{
	Collider::Collider(Type type) : m_type(type)
	{
	}


	Collider::~Collider()
	{
	}

	Collider * Collider::GetNullInstance()
	{
		//static object will be created once, then every time
		// you try to create it again, it will simply reference
		//the one that was created

		static Collider collder(Type::NONE);
		return &collder;
	}

	bool Collider::SPhereToSphereIntersect(const SphereCollider * objA, const SphereCollider * objB, IntersectData * intersect)
	{
		glm::vec3 collisionVector = objB->GetPos() - objA->GetPos();
		
		//calculate distance and total of boath radii so that we can tell if we intersect
		float distance = glm::length(collisionVector);		
		float colDistance = objA->GetRadius() + objB->GetRadius();

		//create the collision vector which points from a to b and is the length of the overlap
		collisionVector = glm::normalize(collisionVector) * (colDistance - distance);
		intersect->collisionVector = collisionVector;

		return distance < colDistance;
	}

	
}