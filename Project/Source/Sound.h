#pragma once

#include "SoundSystem.h"

class Sound : public SoundSystem
{
protected:
	virtual void CreateSound(char* _fileName) {};

	FMOD::Sound *m_pSound;
	FMOD::Channel *m_pChannel;

private:

public:
	Sound() {};
	Sound(char* _fileName) {};
	virtual void PlaySound(glm::vec3 _pos, glm::vec3 _vel, float _volume) {};
	//virtual void PlaySound(float _volume);
	
};