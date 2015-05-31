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
	void PlaySound();

};