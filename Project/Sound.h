#pragma once

#include "Source/SoundSystem.h"

class Sound : public SoundSystem
{
private:
public:
	virtual void CreateSound(char* fileName, glm::vec3 pos, glm::vec3 vel);
	virtual void PlaySound();
};