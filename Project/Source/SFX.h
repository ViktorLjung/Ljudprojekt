#pragma once

#include "Sound.h"

class SFX : public Sound
{
private:
	void CreateSound(char* _fileName) override;
public:
	SFX() {};
	SFX(char* _fileName)
		: Sound(_fileName) {
		CreateSound(_fileName);
	};

	void PlaySound(glm::vec3 _pos, float _volume) override;
	void Update(glm::vec3 _pos);
	bool IsPlaying();

};