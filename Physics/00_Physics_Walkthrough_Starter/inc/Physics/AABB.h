#pragma once
#include "Physics\Collider.h"
#include <glm\vec3.hpp>

namespace Physics
{
	class AABB : public Collider
	{
	public:

		struct  Box
		{
			//position of top left corner
			float x, y, z;

			//dimenstions
			float w, h, d;

			//velocity
			float vx, vy, vz;
		};

		AABB();
		AABB(glm::vec3 size);
		~AABB();

		float SweptAABB(Box b1, Box b2, float normalx, float normaly, float normalz);

		glm::vec3 GetPos() { return m_position; }
		glm::vec3 GetSize() { return m_size; }
		Box GetBox() { return m_box; }

		void SetPos(glm::vec3 p_position) { m_position = p_position; }
		void SetSize(glm::vec3 size) { m_size = size; }
		void SetBox(float w, float h, float d);

	protected:
		glm::vec3 m_size;
		glm::vec3  m_position;
		Box m_box;
	};
}
