#ifndef OBJECT_H
#define OBJECT_H
#include "PrecompiledHeader.h"
#include <btBulletDynamicsCommon.h>
#include "Model.h"
#include "SFX.h"

class Object
{
public:
	Object(btCollisionShape* shape, std::string modelPath);

	Object(btCollisionShape* shape, std::string modelPath, char* sound,
		btScalar mass, btScalar restitution, glm::vec3 position, glm::quat orientation);

	Object(btCollisionShape* shape, std::shared_ptr<Model>  model, char* sound,
		btScalar mass, btScalar restitution, glm::vec3 position, glm::quat orientation);

	void Update(double dt);
	std::shared_ptr<Model>  m_Model;


	btScalar m_Mass; // Mass = 0 = static
	btRigidBody* m_RigidBody;
	btCollisionShape* m_Shape;

	char* m_Sound;

	btScalar m_Restitution;
	glm::vec3 m_Position;
	glm::quat m_Orientation;
	glm::vec3 m_Scale;

	void Boop(float appImpulse);
	//SFX boop;

	void ModelMatrix(glm::vec3 position, glm::quat orientation, glm::vec3 scale);
	void ModelMatrix(glm::mat4 trans);
	glm::mat4 ModelMatrix();
private:
	

	glm::mat4 m_ModelMatrix;
};
#endif