#pragma once

#include "Sound.h"

class SFX : public Sound
{
private:
public:
	SFX(FMOD::System* _system)
		: Sound(_system) {
		//CreateSound(_fileName);
	};

	void CreateSound(char* _fileName) override;

	void PlaySound(glm::vec3 _pos, float _volume) override;
	void Update(glm::vec3 _pos);
	bool IsPlaying();
	FMOD_VECTOR pos;

};