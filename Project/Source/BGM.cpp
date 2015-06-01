#include "BGM.h"

void BGM::CreateSound(char* _fileName)
{
	m_pSystem->createSound(_fileName, FMOD_2D, 0, &m_pSound);
	m_pSound->setMode(FMOD_LOOP_NORMAL);
}

void BGM::PlaySound(float _volume)
{
	m_pSystem->playSound(FMOD_CHANNEL_FREE, m_pSound, true, &m_pChannel);
	m_pChannel->setVolume(_volume);
	m_pChannel->setPaused(false);
}

void BGM::StopSound()
{
	m_pChannel->stop();
}

bool BGM::IsPlaying()
{
	bool b;
	m_pChannel->isPlaying(&b);
	return b;
}