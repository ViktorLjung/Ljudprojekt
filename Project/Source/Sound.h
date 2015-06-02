#pragma once
#include "PrecompiledHeader.h"
#include <map>
#include "fmod.hpp"

class Sound 
{
protected:
	FMOD::System *m_pSystem;
	FMOD::Sound *m_pSound;
	FMOD::Channel *m_pChannel;


private:

public:
	virtual void CreateSound(char* _fileName) {};
	Sound(FMOD::System *_system) { m_pSystem = _system; };
	virtual void PlaySound(glm::vec3 _pos, float _volume) {};
	virtual void PlaySound(float _volume) {};
	FMOD::Channel* GetChannel() { return m_pChannel; };
	FMOD::Sound* GetSound() { return m_pSound; };

	//virtual void PlaySound(float _volume);
	
};