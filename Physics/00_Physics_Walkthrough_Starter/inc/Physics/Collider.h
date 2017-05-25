#pragma once
#include "Intersect.h"

namespace Physics
{
	class PhysicsObject;
	class SphereCollider;

	class Collider
	{
	public:

		enum class Type
		{
			NONE,
			SPHERE,
			AABB,
			OBB
		};

		

		Collider(Type type);
		virtual ~Collider();

		Type GetType() const {	return m_type;	};

		virtual void Transform(PhysicsObject* p_object) {};

		virtual bool Intersects(Collider* other, IntersectData* info) const { return false;	};

		//Static functio now matter how many colliders are created
		static Collider* GetNullInstance();

		static bool SPhereToSphereIntersect(const SphereCollider *objA, const SphereCollider *objB, IntersectData * intersect);

	protected:
		Type m_type;

	private:
	};
}
