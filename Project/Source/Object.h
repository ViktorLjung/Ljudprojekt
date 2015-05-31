#include "PrecompiledHeader.h"
#include <btBulletDynamicsCommon.h>
#include "Model.h"

class Object
{
public:
	Object();

private:
	bool m_Static;
	btRigidBody* m_RigidBody;
	btCollisionShape* m_Shape;
	Model* m_Model;

	glm::vec3 m_Position;
	glm::mat4 m_Orientation;

};
