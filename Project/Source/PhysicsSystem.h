#include "PrecompiledHeader.h"
#include <btBulletDynamicsCommon.h>

class PhysicsSystem
{
public:
	PhysicsSystem();

	void Initialize();
	void Update(double dt);
	void CleanUp();


	btRigidBody* AddRigidBody(btCollisionShape* shape, btScalar mass, glm::vec3 position, glm::mat4 transform);

private:


	btDefaultCollisionConfiguration* m_CollisionConfig;
	btCollisionDispatcher* m_Dispatcher;
	btBroadphaseInterface* m_Broadface;
	btSequentialImpulseConstraintSolver* m_Solver;
	btDiscreteDynamicsWorld* m_World;


	btAlignedObjectArray<btCollisionShape*> m_CollisionShapes;
};
