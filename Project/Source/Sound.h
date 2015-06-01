#pragma once

#include "SoundSystem.h"

class Sound : public SoundSystem
{
protected:
	FMOD::Sound *m_pSound;
	
private:
	virtual void CreateSound(char* _fileName);
public:
	Sound();
	Sound(char* _fileName);
	virtual void PlaySound(glm::vec3 _pos, float _volume);
	virtual void PlaySound(float _volume);
	
	
};