#include "PrecompiledHeader.h"
//#include <btBulletDynamicsCommon.h>
#include "Object.h"
#include <list>

class ObjectManager
{
public:
	ObjectManager();

	
	std::list<Object*> GetObjects() const { return Objects; }

	void AddObject(Object* object);

	void Update(double dt);
private:
	std::list<Object*> Objects;

};
