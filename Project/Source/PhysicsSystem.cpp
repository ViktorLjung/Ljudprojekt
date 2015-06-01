#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
{

}

void PhysicsSystem::Initialize()
{
	m_CollisionConfig = new btDefaultCollisionConfiguration();
	m_Dispatcher = new btCollisionDispatcher(m_CollisionConfig);
	m_Broadface = new btDbvtBroadphase();
	m_Solver = new btSequentialImpulseConstraintSolver();
	m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadface, m_Solver, m_CollisionConfig);

	m_World->setGravity(btVector3(0, -9.82, 0));
}

btRigidBody* PhysicsSystem::AddRigidBody(btCollisionShape* shape, btScalar mass, btScalar restitution, glm::mat4 transformGL)
{
	btTransform transform;
	transform.setFromOpenGLMatrix(glm::value_ptr(transformGL));
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);


	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	rbInfo.m_restitution = restitution;
	rbInfo.m_friction = 1.5f;
	btRigidBody* body = new btRigidBody(rbInfo);


	m_World->addRigidBody(body);
	return body;
}


void PhysicsSystem::Update(double dt)
{
	m_World->stepSimulation(1.f / 60.f, 10);

	for (int j = m_World->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* object = m_World->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(object);
		btTransform trans;

		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = object->getWorldTransform();
		}
		//printf(" world   pos  object  %d = %f ,%f ,%f\n", j, float(trans.getOrigin().getX()), float(
		//	trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}

}

void PhysicsSystem::CheckCollisions()
{
	int numManifolds = m_World->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = m_World->getDispatcher()->getManifoldByIndexInternal(i);

		btCollisionObject* obA = (btCollisionObject*)contactManifold->getBody0();

		btCollisionObject* obB = (btCollisionObject*)contactManifold->getBody1();
		btRigidBody* body1 = btRigidBody::upcast(obA);
		btRigidBody* body2 = btRigidBody::upcast(obB);

		int numContacts = contactManifold->getNumContacts();

		for (int j = 0; j < numContacts; j++)

		{
			body1->checkCollideWith(body2);

			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			if (pt.getDistance() < 0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();

				const btVector3& ptB = pt.getPositionWorldOnB();

				const btVector3& normalOnB = pt.m_normalWorldOnB;
				//printf("Collision!!!");
			}
		}
	}
}

void PhysicsSystem::RemoveRigidBody(btCollisionShape* shape, btRigidBody* body)
{
	if (body && body->getMotionState())
	{
		delete body->getMotionState();
	}
	
	m_World->removeRigidBody(body);
	delete body;
}

void PhysicsSystem::CleanUp()
{
	for (int i = m_World->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* object = m_World->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(object);

		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_World->removeCollisionObject(object);
		delete object;
	}

	//Delete collision shapes
	//delete collision shapes
	

	delete m_World;
	delete m_Solver;
	delete m_Broadface;
	delete m_Dispatcher;
	delete m_CollisionConfig;

}
