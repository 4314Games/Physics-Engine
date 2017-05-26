#include "Physics\Constraint.h"

using namespace Physics;

Physics::Constraint::Constraint(std::vector<PhysicsObject*>& objects, Type type) :m_objects(objects),m_type(type)
{
	
}

Constraint::~Constraint()
{
}
