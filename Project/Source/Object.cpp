#include "Object.h"

Object::Object(btCollisionShape* shape)
{
	m_Mass = 0;
	m_Position = glm::vec3(0, 0, 0);
	m_Orientation = glm::mat4();
	m_Scale = glm::vec3(1, 1, 1);
	m_PhysicsOrigin = glm::vec3(1, 1, 1);

	m_Shape = shape;
}
