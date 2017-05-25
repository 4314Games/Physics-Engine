#include "Physics\Collider.h"
#include "Physics\SphereCollider.h"
#include "Physics\PhysicsScene.h"

#include <glm\vec3.hpp>
#include<glm\geometric.hpp>

using namespace Physics;

Collider::Collider(Type type) : m_type(type)
{
}


Collider::~Collider()
{
}

Collider * Physics::Collider::GetNullInstance()
{
	// static object will be created once, then every time
	// you try to create it again, it will simply reference
	// the one that was created.
	static Collider collider(Type::NONE);
	return &collider;
}

bool Physics::Collider::SphereToSphereIntersect(const SphereCollider * objA, const SphereCollider * objB, IntersectData* info)
{
	glm::vec3 collisionVector = objB->GetPosition() - objA->GetPosition();
	
	// calculate distance and total of both radii so that we can tell if we intersect
	float distance = glm::length(collisionVector);
	float colDistance = objA->GetRadius() + objB->GetRadius();

	// create the collision vector which points from A to B and is the length of the overlap
	collisionVector = glm::normalize(collisionVector) * (colDistance - distance);
	info->collisionVector = collisionVector;

	return distance < colDistance;

}


