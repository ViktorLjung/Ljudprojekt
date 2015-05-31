#include "PrecompiledHeader.h"
#include <btBulletDynamicsCommon.h>
#include "Model.h"

class Object
{
public:
	Object(btCollisionShape* shape);

private:
	btScalar m_Mass; // Mass = 0 = static
	btRigidBody* m_RigidBody;
	btCollisionShape* m_Shape;
	glm::vec3 m_PhysicsOrigin;


	Model* m_Model;

	glm::vec3 m_Position;
	glm::mat4 m_Orientation;
	glm::vec3 m_Scale;

};
