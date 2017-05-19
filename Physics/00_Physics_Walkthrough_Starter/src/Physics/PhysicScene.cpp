#include "..\..\inc\Physics\PhysicScene.h"
#include "Physics\PhysicsObject.h"
#include <algorithm>
#include "Physics\Collider.h"

namespace Physics
{

	PhysicScene::PhysicScene()
	{
	}


	PhysicScene::~PhysicScene()
	{
		for (auto iter = m_object.begin(); iter != m_object.end(); iter++)
		{
			delete *iter;
		}
		m_object.clear();
	}

	void Physics::PhysicScene::Update(float deltaTime)
	{
		//Applys physics to all objects in scene
		for (auto iter = m_object.begin(); iter != m_object.end(); iter++)
		{
			PhysicsObject* obj = *iter;
			obj->ApplyForce(m_globalForce);
			obj->Update(deltaTime);

			//Dodgy Bounce Physics
			glm::vec3 pos = obj->GetPos();
			glm::vec3 vel = obj->GetVelocity();

			if (pos.y < 0.0f)
			{
				obj->SetPos(glm::vec3(pos.x, 0, pos.z));
				obj->SetVelocty(glm::vec3(vel.x, -vel.y, vel.z));
			}
		}	

		DetectCollision();
	}

	void PhysicScene::ApplyGlobalForce(const glm::vec3 & p_force)
	{
		m_globalForce = p_force;
	}

	void Physics::PhysicScene::AttachObject(PhysicsObject * p_obj)
	{
		auto iter = std::find(m_object.begin(), m_object.end(), p_obj);
		if (iter == m_object.end())	//object pointer is not already in our vecrot
		{
			m_object.push_back(p_obj);
		}
		//m_object.push_back(p_obj);
	}

	void Physics::PhysicScene::RemoveObject(PhysicsObject * p_obj)
	{
		auto iter = std::find(m_object.begin(), m_object.end(), p_obj);
		if (iter != m_object.end())	//object pointer is in our vector
		{
			m_object.erase(iter);
		}
	}

	const std::vector<PhysicsObject*>& PhysicScene::GetObjects() const	//Returns vectors of objects in scene
	{
		return m_object;
	}

	bool PhysicScene::isObjectColliding(PhysicsObject * object)
	{
		for (auto iter = m_collision.begin(); iter != m_collision.end(); iter++)
		{
			if ((*iter).objA == object || (*iter).objB == object)
			{
				return true;
			}
		}
		return false;
	}
	void PhysicScene::DetectCollision()
	{
		m_collision.clear();
		for (auto iterA = m_object.begin(); iterA != m_object.end(); iterA++)
		{
			PhysicsObject* objA = *iterA;
			for (auto iterB = iterA + 1; iterB != m_object.end(); iterB++)
			{
				PhysicsObject* objB = *iterB;
				if (objA->GetCollider()->Intersects(objB->GetCollider()))
				{
					CollisionInfo info;
					info.objA = objA;
					info.objB = objB;
					m_collision.push_back(info);
				}
			}
		}
	}
}