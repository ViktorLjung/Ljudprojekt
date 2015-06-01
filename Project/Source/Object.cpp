#include "Object.h"

Object::Object(btCollisionShape* shape, std::string modelPath)
{
	m_Model = std::make_shared<Model>(modelPath);
	m_Mass = 1;
	m_Position = glm::vec3(0, 0, 0);
	m_Orientation = glm::quat();
	m_Scale = glm::vec3(1, 1, 1);
	m_Shape = shape;

}

void Object::Update(double dt)
{
	btTransform trans;

	if (m_RigidBody && m_RigidBody->getMotionState())
	{
		m_RigidBody->getMotionState()->getWorldTransform(trans);
	}
	m_Position.x = float(trans.getOrigin().getX());
	m_Position.z = float(trans.getOrigin().getZ());
	m_Position.y = float(trans.getOrigin().getY());
	


	m_Model->ModelMatrix(m_Position, m_Orientation, m_Scale);
}

