#include "PrecompiledHeader.h"
#include <btBulletDynamicsCommon.h>
#include <list>
#include <tuple>

class PhysicsSystem
{
public:
	PhysicsSystem();

	void Initialize();
	void Update(double dt);
	void CleanUp();
	std::list<std::tuple<btRigidBody*, btRigidBody*, btManifoldPoint>> CheckCollisions();

	btRigidBody* AddRigidBody(btCollisionShape* shape, btScalar mass, btScalar restitution, glm::mat4 transformGL);
	void RemoveRigidBody(btCollisionShape* shape, btRigidBody* body);



private:
	btDefaultCollisionConfiguration* m_CollisionConfig;
	btCollisionDispatcher* m_Dispatcher;
	btBroadphaseInterface* m_Broadface;
	btSequentialImpulseConstraintSolver* m_Solver;
	btDiscreteDynamicsWorld* m_World;


};
