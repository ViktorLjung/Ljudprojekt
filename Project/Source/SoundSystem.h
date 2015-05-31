#pragma once
#include "PrecompiledHeader.h"
#include <string>

#include "fmod.hpp"
#include "fmod_errors.h"

class SoundSystem
{
private:
	void Initialize();
	FMOD::System *m_pSystem;
	
public:
	SoundSystem();
	void PlaySound();

};