#include "Sound.h"

void Sound::CreateSound(char* _fileName)
{
	m_pSystem->createSound(_fileName, FMOD_HARDWARE, 0, &m_pSound);
}

void Sound::PlaySound(glm::vec3 _pos, float _volume)
{
	m_pChannel->setVolume(_volume);
	m_pSystem->playSound(FMOD_CHANNEL_FREE, m_pSound, false, &m_pChannel);
}