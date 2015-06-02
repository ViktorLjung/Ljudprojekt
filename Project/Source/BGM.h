#pragma once

#include "Sound.h"

class BGM : public Sound
{
private:
	void CreateSound(char* _fileName);
public:
	BGM(FMOD::System* _system)
		: Sound(_system) {  
		//CreateSound(_system);
	};
	//void PlaySound(float _volume);

	void PlaySound(float _volume) override;
	void StopSound();
	bool IsPlaying();
	
};