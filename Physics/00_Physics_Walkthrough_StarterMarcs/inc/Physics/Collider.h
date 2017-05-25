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

		Type GetType() const { return m_type; };

		virtual void Transform(PhysicsObject* obj) { /* Intentionally left blank */};
		virtual bool Intersects(Collider* other, IntersectData* intersect) const { return false; };

		// static functions: A single function no matter how many
		// colliders are created
		static Collider* GetNullInstance();

		static bool SphereToSphereIntersect(const SphereCollider *objA, const SphereCollider *objB, IntersectData* intersect);

	protected:
		Type m_type;
	private:
	};
}

