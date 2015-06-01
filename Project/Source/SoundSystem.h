#pragma once
#include "PrecompiledHeader.h"
#include <string>

#include "fmod.hpp"
#include "fmod_errors.h"

class SoundSystem
{
protected:
	FMOD::System *m_pSystem;
	FMOD::Channel *m_pChannel;

private:
	void Initialize();
	
public:
	SoundSystem();
	void Update(glm::vec3 lisPos, glm::vec3 lisVel, glm::vec3 lisFor, glm::vec3 lisUp);
	
};