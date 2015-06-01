#include "SoundSystem.h"


SoundSystem::SoundSystem()
{
	Initialize();
}

void SoundSystem::Initialize()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(36, FMOD_INIT_3D_RIGHTHANDED | FMOD_INIT_ENABLE_PROFILE, NULL);
	m_pSystem->set3DSettings(1.f, 1.f, 1.f);
}

void SoundSystem::Update(glm::vec3 lisPos, glm::vec3 lisVel, glm::vec3 lisFor, glm::vec3 lisUp)
{
	m_pSystem->update();
	FMOD_VECTOR p = { lisPos.x, lisPos.y, lisPos.z };
	FMOD_VECTOR v = { lisVel.x, lisVel.y, lisVel.z };
	FMOD_VECTOR f = { lisFor.x, lisFor.y, lisFor.z };
	FMOD_VECTOR u = { lisUp.x,  lisUp.y,  lisUp.z  };
	//Update listener
	m_pSystem->set3DListenerAttributes(0, &p, &v, &f, &u);
}
