#include "..\..\inc\Physics\PhysicsRenderer.h"
#include <vector>
#include "Physics\PhysicScene.h"
#include "Physics\PhysicsObject.h"
#include <glm/glm.hpp>
#include "Physics\Collider.h"
#include "Physics\SphereCollider.h"

namespace Physics
{

	PhysicsRenderer::PhysicsRenderer()
	{
	}

	PhysicsRenderer::~PhysicsRenderer()
	{
	}

	void Physics::PhysicsRenderer::Draw(PhysicScene * p_scene)
	{
		RenderGizmosPhysics(p_scene);
	}

	void PhysicsRenderer::RenderGizmosPhysics(PhysicScene * p_scene)	//Render Physics Gizmos
	{
		const std::vector<Physics::PhysicsObject*>  & objects = p_scene->GetObjects();
		for (auto iter = objects.begin(); iter != objects.end(); iter++)
		{
			Physics::PhysicsObject *obj = *iter;
			RenderInfo *info = GetRenderInfro(obj);
		
			glm::vec4 renderColor = info->color;

			if (p_scene->isObjectColliding(obj))
			{
				renderColor = glm::vec4(1, 0, 0, 1);
			}
			switch (obj->GetCollider()->GetType())
			{
			case Collider::Type::SPHERE:	//Draw the collider actual size sphere
				aie::Gizmos::addSphere(obj->GetPos(), ((SphereCollider*)obj->GetCollider())->GetRadius(), 10, 10, renderColor);
			case Collider::Type::NONE:	//no collider, just show a small sphere
				aie::Gizmos::addSphere(obj->GetPos(), ((SphereCollider*)obj->GetCollider())->GetRadius(), 5, 5, info->color);
			}			
		}
	}

	PhysicsRenderer::RenderInfo * Physics::PhysicsRenderer::GetRenderInfro(PhysicsObject * p_object)
	{
		return &m_renderInfo[p_object];
	}
}
