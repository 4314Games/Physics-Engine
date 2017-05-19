#include "..\..\inc\Physics\Collider.h"
#include "Physics\SphereCollider.h"
#include <glm\vec3.hpp>
#include <glm\geometric.hpp>

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

	bool Collider::SPhereToSphereIntersect(const SphereCollider * objA, const SphereCollider * objB)
	{
		glm::vec3 vSpheres = objA->GetPos() - objB->GetPos();
		float distance = glm::length(vSpheres);		

		float colDistance = objA->GetRadius() + objB->GetRadius();

		return distance < colDistance;
	}

	
}