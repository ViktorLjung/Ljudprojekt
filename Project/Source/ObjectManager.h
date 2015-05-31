#include "PrecompiledHeader.h"
//#include <btBulletDynamicsCommon.h>
#include "Object.h"
#include <list>

class ObjectManager
{
public:
	ObjectManager();

	std::list<Object*> Objects;

	void Update(double dt);
private:

};
