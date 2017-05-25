#pragma once
#include <map>
#include <glm/vec4.hpp>

namespace Physics
{
	class PhysicScene;
	class PhysicsObject;

	class PhysicsRenderer
	{
	public:
		
		struct RenderInfo
		{
			glm::vec4 color = glm::vec4(1,1,1,1);			
		};
		
		PhysicsRenderer();
		virtual ~PhysicsRenderer();

		virtual void Draw(PhysicScene * scene);
		RenderInfo* GetRenderInfro(PhysicsObject* p_object);

	protected:
		std::map<PhysicsObject*, RenderInfo> m_renderInfo;

	private:
		void RenderGizmosPhysics(PhysicScene* p_scene);
		
	};


}
