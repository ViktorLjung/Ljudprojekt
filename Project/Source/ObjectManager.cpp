#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	btCollisionShape* shape = new btBoxShape(btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));
	Objects.push_back(new Object(shape, "Assets/Models/Cube.obj"));
	Objects.push_back(new Object(shape, "Assets/Models/Cube.obj", 1, glm::vec3(0.75, 5, 0), glm::quat(), glm::vec3(1)));
}

void ObjectManager::Update(double dt)
{
	for (auto i = Objects.begin(); i != Objects.end(); i++)
	{
		(*i)->Update(dt);
	}
}
