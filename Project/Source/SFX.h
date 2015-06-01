#pragma once

#include "Sound.h"

class SFX : public Sound
{
public:
	SFX();
	SFX(char* _fileName)
		: Sound(_fileName) {};

};