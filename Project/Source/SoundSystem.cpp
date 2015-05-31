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
