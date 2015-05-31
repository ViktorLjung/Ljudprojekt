#include <btBulletDynamicsCommon.h>

class PhysicsSystem
{
public:
	PhysicsSystem();

	void Initialize();
	void Update(double dt);
	void CleanUp();
private:


	btDefaultCollisionConfiguration* m_CollisionConfig;
	btCollisionDispatcher* m_Dispatcher;
	btBroadphaseInterface* m_Broadface;
	btSequentialImpulseConstraintSolver* m_Solver;
	btDiscreteDynamicsWorld* m_World;


	btAlignedObjectArray<btCollisionShape*> m_CollisionShapes;
};
