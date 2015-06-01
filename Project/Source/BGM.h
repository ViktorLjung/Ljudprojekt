#pragma once

#include "Sound.h"

class BGM : public Sound
{
private:
	void CreateSound(char* _fileName);
public:
	BGM() {};
	BGM(char* _fileName) 
		: Sound(_fileName) {  
		CreateSound(_fileName);
	};
	//void PlaySound(float _volume);

	void PlaySound(float _volume) override;
	void StopSound();
	bool IsPlaying();
	
};