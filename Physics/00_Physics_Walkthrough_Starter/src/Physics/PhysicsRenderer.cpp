#include "..\..\inc\Physics\PhysicsRenderer.h"
#include "Physics\PhysicScene.h"
#include "Physics\PhysicsObject.h"
#include "Physics\Collider.h"
#include "Physics\SphereCollider.h"
#include "Physics\Constraint.h"
#include "Physics\AABB.h"

#include <vector>
#include <glm/glm.hpp>

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

	void PhysicsRenderer::RenderGizmosPhysics(PhysicScene * p_scene)			//Render Physics/Scene Gizmos
	{
		const std::vector<Physics::PhysicsObject*>  & objects = p_scene->GetObjects();
		
		for (auto iter = objects.begin(); iter != objects.end(); iter++)		//Render all the objects in the scene
		{
			Physics::PhysicsObject *obj = *iter;
			RenderInfo *info = GetRenderInfro(obj);
		
			glm::vec4 renderColor = info->color;

			if (p_scene->isObjectColliding(obj))								//If Colliding change color to red
			{
				renderColor = glm::vec4(1, 0, 0, 1);
			}
			switch (obj->GetCollider()->GetType())
			{
			case Collider::Type::SPHERE:																							//Draw the collider actual size sphere
				aie::Gizmos::addSphere(obj->GetPos(), ((SphereCollider*)obj->GetCollider())->GetRadius(), 10, 10, renderColor);
				break;
			case Collider::Type::AABB:																								//Draw AABB Box
				aie::Gizmos::addAABB(obj->GetPos(), ((AABB*)obj->GetCollider())->GetSize(), renderColor);	
				break;
			case Collider::Type::NONE:																								//no collider, just show a small sphere
				aie::Gizmos::addSphere(obj->GetPos(), ((SphereCollider*)obj->GetCollider())->GetRadius(), 5, 5, info->color);
			}			
		}

		for (auto iter = p_scene->GetConstraints().begin(); iter != p_scene->GetConstraints().end(); iter++)	//Render spring connections
		{
			switch ((*iter)->GetType())
				case Constraint::Type::SPRING:
					aie::Gizmos::addLine((*iter)->GetObjects()[0]->GetPos(), (*iter)->GetObjects()[1]->GetPos(), glm::vec4(1, 0, 0, 1));		
		}

	}

	PhysicsRenderer::RenderInfo * Physics::PhysicsRenderer::GetRenderInfro(PhysicsObject * p_object)
	{
		return &m_renderInfo[p_object];
	}
}
