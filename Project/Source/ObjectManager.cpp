#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	
}

void ObjectManager::Update(double dt)
{
	for (auto i = Objects.begin(); i != Objects.end(); i++)
	{
		(*i)->Update(dt);
	}
}

void ObjectManager::AddObject(Object* object)
{
	Objects.push_back(object);
}
