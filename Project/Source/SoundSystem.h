#pragma once
#include "PrecompiledHeader.h"
#include <string>
#include <vector>

#include "fmod.hpp"
#include "fmod_errors.h"

#include "SFX.h"

class SoundSystem
{
protected:
	FMOD::System *m_pSystem;

private:
	std::vector<SFX*> sfxList;
	void Initialize();
	std::map<char*, SFX*> cachedSounds;

	
public:
	SoundSystem();
	void Update(glm::vec3 lisPos, glm::vec3 lisFor, glm::vec3 lisUp);

	void PlaySFX(char* _filename, float _volume, glm::vec3 pos);
	
};