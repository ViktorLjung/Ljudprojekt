#include "PrecompiledHeader.h"
#include <btBulletDynamicsCommon.h>
#include "Model.h"

class Object
{
public:
	Object(btCollisionShape* shape, std::string modelPath);
	void Update(double dt);
	std::shared_ptr<Model>  m_Model;


	btScalar m_Mass; // Mass = 0 = static
	btRigidBody* m_RigidBody;
	btCollisionShape* m_Shape;

	glm::vec3 m_Position;
	glm::quat m_Orientation;
	glm::vec3 m_Scale;
private:
	

};
