#include "Object.h"

Object::Object(btCollisionShape* shape, std::string modelPath)
{
	m_Model = std::make_shared<Model>(modelPath);
	m_Mass = 0;
	m_Position = glm::vec3(0, 0, 0);
	m_Orientation = glm::quat();
	m_Scale = glm::vec3(1);
	m_Shape = shape;
	m_Model->ModelMatrix(m_Position, m_Orientation, m_Scale);
}
Object::Object(btCollisionShape* shape, std::string modelPath, 
	btScalar mass, glm::vec3 position, glm::quat orientation)
{
	m_Model = std::make_shared<Model>(modelPath);
	m_Mass = mass;
	m_Position = position;
	m_Orientation = orientation;
	m_Scale = glm::vec3(1);
	m_Shape = shape;
	m_Model->ModelMatrix(m_Position, m_Orientation, m_Scale);
}


void Object::Update(double dt)
{
	btTransform trans;

	if (m_RigidBody && m_RigidBody->getMotionState())
	{
		m_RigidBody->getMotionState()->getWorldTransform(trans);
	}

	glm::mat4 transformGL;
	trans.getOpenGLMatrix(glm::value_ptr(transformGL));
	
	
	m_Model->ModelMatrix(transformGL);
	
}

