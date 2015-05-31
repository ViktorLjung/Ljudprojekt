#include "SoundSystem.h"

void SoundSystem::PlaySound()
{
	FMOD::System *m_pSystem;
	FMOD::System_Create(&m_pSystem);

	m_pSystem->init(36, FMOD_INIT_NORMAL, NULL);

	std::string wavFile = "Assets/Sounds/test.wav";

	FMOD::Sound *sound;
	m_pSystem->createSound(wavFile.c_str(), FMOD_HARDWARE, 0, &sound);

	m_pSystem->playSound(FMOD_CHANNEL_FREE, sound, false, 0);
}