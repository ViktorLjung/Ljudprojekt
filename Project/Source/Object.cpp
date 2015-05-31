#include "Object.h"

Object::Object(btCollisionShape* shape, std::string modelPath)
{
	m_Model = std::make_shared<Model>(modelPath);
	m_Mass = 0;
	m_Position = glm::vec3(0, 0, 0);
	m_Orientation = glm::quat();
	m_Scale = glm::vec3(1, 1, 1);
	m_PhysicsOrigin = glm::vec3(1, 1, 1);

	m_Shape = shape;

	scaler = 1.f;
}

void Object::Update(double dt)
{

	if (m_Scale.y > 2.f)
		scaler *= -1;
	else if (m_Scale.y < 0.1f)
		scaler *= -1;



	m_Scale += scaler *dt;
	m_Model->ModelMatrix(m_Position, m_Orientation, m_Scale);
}

