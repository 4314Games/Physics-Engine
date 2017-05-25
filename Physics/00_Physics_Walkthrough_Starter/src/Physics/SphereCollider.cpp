#include "Physics\SphereCollider.h"

namespace Physics
{

	SphereCollider::SphereCollider() : m_radius(1.0f), Collider(Type::SPHERE)
	{
	}

	SphereCollider::SphereCollider(float p_radius) : m_radius(p_radius), Collider(Type::SPHERE)
	{
	}

	SphereCollider::~SphereCollider()
	{
	}

	bool SphereCollider::Intersects(Collider * other, IntersectData * intersect) const
	{
		switch (other->GetType())
		{
		case Type::SPHERE:
			return SPhereToSphereIntersect(this, (SphereCollider*)other, intersect);		
		}
		return false;
	}

	void SphereCollider::Transform(PhysicsObject * p_obj)
	{
		SetPosition(p_obj->GetPos());
	}
}