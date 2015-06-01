#include "SFX.h"


void SFX::CreateSound(char* _fileName)
{
	m_pSystem->createSound(_fileName, FMOD_3D, 0, &m_pSound);
	m_pSound->setMode(FMOD_LOOP_OFF);
}

void SFX::PlaySound(glm::vec3 _pos, float _volume)
{
	FMOD_VECTOR p = { _pos.x, _pos.y, _pos.z };
	FMOD_VECTOR v = { 0, 0, 0 };
	//m_pSound->set3DMinMaxDistance(0.f, 1.f);
	m_pChannel->set3DAttributes(&p, &v);
	m_pSystem->playSound(FMOD_CHANNEL_FREE, m_pSound, true, &m_pChannel);
	m_pChannel->setVolume(_volume);
	m_pChannel->setPaused(false);
}

void SFX::Update(glm::vec3 _pos)
{

}