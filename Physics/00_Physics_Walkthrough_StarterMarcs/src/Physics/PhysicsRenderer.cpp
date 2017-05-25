#include "Physics\PhysicsRenderer.h"
#include "Physics\PhysicsObject.h"
#include "Physics\PhysicsScene.h"
#include "Physics\Collider.h"
#include "Physics\SphereCollider.h"

#include <vector>

#include <glm\vec4.hpp>

#include <Gizmos.h>

using namespace Physics;

PhysicsRenderer::PhysicsRenderer()
{
}


PhysicsRenderer::~PhysicsRenderer()
{
}

void Physics::PhysicsRenderer::Draw(PhysicsScene * scene)
{
	RenderGizmosPhysics(scene);

}

void PhysicsRenderer::RenderGizmosPhysics(PhysicsScene * scene)
{
	const std::vector<Physics::PhysicsObject*> & objects = scene->GetObjects();
	for (auto iter = objects.begin(); iter != objects.end(); iter++)
	{
		Physics::PhysicsObject *obj = *iter;
		RenderInfo *info = GetRenderInfo(obj);

		glm::vec4 renderColor = info->color;

		if (scene->isObjectColliding(obj))
		{
			renderColor = glm::vec4(1, 0, 0, 1);
		}

		switch (obj->GetCollider()->GetType())
		{
		case Collider::Type::SPHERE: // Draw the collider's actual size sphere
			aie::Gizmos::addSphere(obj->GetPosition(), ((SphereCollider*)obj->GetCollider())->GetRadius(), 10, 10, renderColor);
		case Collider::Type::NONE: // No collider, just show a small sphere
			aie::Gizmos::addSphere(obj->GetPosition(), 0.2f, 10, 10, info->color);
		}
		
	}
}

PhysicsRenderer::RenderInfo * Physics::PhysicsRenderer::GetRenderInfo(PhysicsObject * obj)
{
	return &m_renderInfo[obj];
}
