#pragma once

#include "SoundSystem.h"

class Sound : public SoundSystem
{
protected:
	FMOD::Sound *m_pSound;
	
private:
public:
	virtual void PlaySound(glm::vec3 _pos, float _volume);
	virtual void CreateSound(char* _fileName);
	
};