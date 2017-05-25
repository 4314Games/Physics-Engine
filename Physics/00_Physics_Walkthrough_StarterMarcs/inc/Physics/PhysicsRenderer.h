#pragma once

#include <glm\vec4.hpp>

#include <map>

namespace Physics
{
	class PhysicsScene;
	class PhysicsObject;

	class PhysicsRenderer
	{
	public:

		struct RenderInfo
		{
			glm::vec4 color = glm::vec4(1, 1, 1, 1);
		};

		PhysicsRenderer();
		virtual ~PhysicsRenderer();

		virtual void Draw(PhysicsScene *scene);

		RenderInfo* GetRenderInfo(PhysicsObject* obj);

	protected:
		std::map<PhysicsObject*, RenderInfo> m_renderInfo;

	private:
		void RenderGizmosPhysics(PhysicsScene * scene);
	};

}

