#include "Physics\AABB.h"
#include <iostream>
#include <algorithm>

using namespace Physics;

AABB::AABB() : Collider(Type::AABB)
{
}

AABB::AABB(glm::vec3 size) : m_size(size), Collider(Type::AABB)
{
	m_box.w = size.x;
	m_box.h = size.y;
	m_box.d = size.z;
}

AABB::~AABB()
{
}

float Physics::AABB::SweptAABB(Box b1, Box b2, float  normalx, float  normaly, float  normalz)
{
	float xInvEntry, yInvEntry, zInvEntry;
	float xInvExit, yInvExit, zInvExit;

	// find the distance between the objects on the near and far sides for both x and y
	if (b1.vx > 0.0f)	//X Point
	{
		xInvEntry = b2.x - (b1.x + b1.w);
		xInvExit = (b2.x + b2.w) - b1.x;
	}
	else
	{
		xInvEntry = (b2.x + b2.w) - b1.x;
		xInvExit = b2.x - (b1.x + b1.w);
	}

	if (b1.vy > 0.0f)	//Y Point
	{
		yInvEntry = b2.y - (b1.y + b1.h);
		yInvExit = (b2.y + b2.h) - b1.y;
	}
	else
	{
		yInvEntry = (b2.y + b2.h) - b1.y;
		yInvExit = b2.y - (b1.y + b1.h);
	}
	if (b1.vz > 0.0f)	//Z Point
	{
		zInvEntry = b2.z - (b1.z + b1.d);
		zInvExit = (b2.z + b2.d) - b1.z;
	}
	else
	{
		zInvEntry = (b2.z + b2.d) - b1.z;
		zInvExit = b2.z - (b1.z + b1.d);
	}

	// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
	float xEntry, yEntry, zEntry;
	float xExit, yExit, zExit;

	if (b1.vx == 0.0f)
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / b1.vx;
		xExit = xInvExit / b1.vx;
	}

	if (b1.vy == 0.0f)
	{
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		yEntry = yInvEntry / b1.vy;
		yExit = yInvExit / b1.vy;
	}

	if (b1.vz == 0.0f)
	{
		zEntry = -std::numeric_limits<float>::infinity();
		zExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		zEntry = zInvEntry / b1.vz;
		zExit = zInvExit / b1.vz;
	}

	// //find the earliest/latest times of collision
	float entryTime = std::max(xEntry, yEntry);
	entryTime = std::max(entryTime, zEntry);
	float exitTime = std::min(xExit, yExit);
	exitTime = std::min(exitTime, zExit);

	// if there was no collision
	if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f || zEntry < 0.0f || zEntry > 1.0f)
	{
		normalx = 0.0f;
		normaly = 0.0f;
		normalz = 0.0f;
		
		return 1.0f;
	}
	else // if there was a collision
	{
		// calculate normal of collided surface
		if (xEntry > yEntry && xEntry > zEntry)
		{
			if (xInvEntry < 0.0f)
			{
				normalx = 1.0f;
				normaly = 0.0f;
				normalz = 0.0f;
			}
			else
			{
				normalx = -1.0f;
				normaly = 0.0f;
				normalz = 0.0f;
			}
		}
		else if(yEntry > zEntry)
		{
			if (yInvEntry < 0.0f)
			{
				normalx = 0.0f;
				normaly = 1.0f;
				normalz = 0.0f;
			}
			else
			{
				normalx = 0.0f;
				normaly = -1.0f;

			}
		}
		else
		{
			if (zInvEntry < 0.0f)
			{
				normalx = 0.0f;
				normaly = 0.0f;
				normalz = 1.0f;
			}
			else
			{
				normalx = 0.0f;
				normaly = 0.0f;
				normalz = -1.0f;
			}
		}
		//return the time of collision
		return entryTime;		
	}	
}

void Physics::AABB::SetBox(float w, float h, float d)
{
	m_box.d = d;
	m_box.w = w;
	m_box.h = h;
	m_box.x = m_position.x + w / 2;
	m_box.y = m_position.y + h / 2;
	m_box.z = m_position.z + d / 2;
}




