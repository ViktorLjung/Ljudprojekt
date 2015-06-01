#include "Object.h"

Object::Object(btCollisionShape* shape, std::string modelPath) //Static
{
	m_Model = std::make_shared<Model>(modelPath);
	m_Mass = 0;
	m_Position = glm::vec3(0, 0, 0);
	m_Orientation = glm::quat();
	m_Scale = glm::vec3(1);
	m_Shape = shape;
	m_ModelMatrix = glm::mat4();
	ModelMatrix(m_Position, m_Orientation, m_Scale);
	m_Restitution = 0;
}
Object::Object(btCollisionShape* shape, std::string modelPath, 
	btScalar mass, btScalar restitution, glm::vec3 position, glm::quat orientation)
{
	m_Model = std::make_shared<Model>(modelPath);
	m_Mass = mass;
	m_Restitution = restitution;
	m_Position = position;
	m_Orientation = orientation;
	m_Scale = glm::vec3(1);
	m_Shape = shape;
	m_ModelMatrix = glm::mat4();
	ModelMatrix(m_Position, m_Orientation, m_Scale);
}

Object::Object(btCollisionShape* shape, std::shared_ptr<Model> model,
	btScalar mass, btScalar restitution, glm::vec3 position, glm::quat orientation)
{
	m_Model = model;
	m_Mass = mass;
	m_Restitution = restitution;
	m_Position = position;
	m_Orientation = orientation;
	m_Scale = glm::vec3(1);
	m_Shape = shape;
	m_ModelMatrix = glm::mat4();
	ModelMatrix(m_Position, m_Orientation, m_Scale);
}


glm::mat4 Object::ModelMatrix()
{
	return m_ModelMatrix;
}

void Object::ModelMatrix(glm::vec3 position, glm::quat orientation, glm::vec3 scale)
{
	m_ModelMatrix = glm::translate(glm::mat4(), position) * glm::toMat4(orientation) * glm::scale(scale);
}

void Object::ModelMatrix(glm::mat4 trans)
{
	m_ModelMatrix = trans;
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
	
	
	ModelMatrix(transformGL);
	
}

