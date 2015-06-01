#include "SoundSystem.h"


SoundSystem::SoundSystem()
{
	Initialize();
}

void SoundSystem::Initialize()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(128, FMOD_3D, NULL);
	m_pSystem->set3DSettings(1.f, 1.f, 1.f);
}

void SoundSystem::Update(glm::vec3 lisPos, glm::vec3 lisFor, glm::vec3 lisUp)
{
	
	FMOD_VECTOR p = { lisPos.x, lisPos.y, lisPos.z };
	FMOD_VECTOR f = { lisFor.x, lisFor.y, lisFor.z };
	FMOD_VECTOR u = { lisUp.x,  lisUp.y,  lisUp.z  };
	//Update listener
	m_pSystem->set3DListenerAttributes(0, &p, NULL, &f, &u);

	p.x += 10;
//	m_pChannel->set3DAttributes(&p, NULL);
	m_pSystem->update();


}
