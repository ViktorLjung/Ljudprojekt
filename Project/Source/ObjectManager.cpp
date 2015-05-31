#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	btCollisionShape* shape = new btSphereShape(1);
	Objects.push_back(new Object(shape, "Assets/Models/SSAOTest3.obj"));
}

void ObjectManager::Update(double dt)
{
	for (auto i = Objects.begin(); i != Objects.end(); i++)
	{
		(*i)->Update(dt);
	}
}
