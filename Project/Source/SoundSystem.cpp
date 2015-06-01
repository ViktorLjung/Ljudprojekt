#include "SoundSystem.h"


SoundSystem::SoundSystem()
{
	Initialize();
}

void SoundSystem::Initialize()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(36, FMOD_INIT_NORMAL, NULL);
}

void SoundSystem::Update(glm::vec3 lisPos, glm::vec3 lisVel, glm::vec3 lisFor, glm::vec3 lisUp)
{
	FMOD_VECTOR p = { lisPos.x, lisPos.y, lisPos.z };
	FMOD_VECTOR v = { lisVel.x, lisVel.y, lisVel.z };
	FMOD_VECTOR f = { lisFor.x, lisFor.y, lisFor.z };
	FMOD_VECTOR u = { lisUp.x, lisUp.y, lisUp.z };
	//Update listener
	m_pSystem->set3DListenerAttributes(0, &p, &v, &f, &u);
	m_pSystem->update();
}
