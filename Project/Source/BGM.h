#pragma once

#include "Sound.h"

class BGM : public Sound
{
public:
	BGM();
	BGM(char* _fileName) 
		: Sound(_fileName) {};
	//void PlaySound(float _volume);
};