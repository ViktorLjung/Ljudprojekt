#include "SFX.h"


void SFX::CreateSound(char* _fileName)
{
	m_pSystem->createSound(_fileName, FMOD_3D, 0, &m_pSound);
}

void SFX::PlaySound(glm::vec3 _pos, glm::vec3 _vel, float _volume)
{
	FMOD_VECTOR p = { _pos.x, _pos.y, _pos.z };
	FMOD_VECTOR v = { _vel.x, _vel.y, _vel.z };

	
	m_pChannel->set3DAttributes(&p, &v);
	m_pSystem->playSound(FMOD_CHANNEL_FREE, m_pSound, true, &m_pChannel);
	m_pChannel->setVolume(_volume);
	m_pChannel->setPaused(false);
}