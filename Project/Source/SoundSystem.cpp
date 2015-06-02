#include "SoundSystem.h"


SoundSystem::SoundSystem()
{
	Initialize();
}

void SoundSystem::Initialize()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(128, FMOD_INIT_3D_RIGHTHANDED, NULL);
	m_pSystem->set3DSettings(1.f, 1.f, 1.f);
}

void SoundSystem::Update(glm::vec3 lisPos, glm::vec3 lisFor, glm::vec3 lisUp)
{
	m_pSystem->update();

	FMOD_VECTOR p = { lisPos.x, lisPos.y, lisPos.z };
	FMOD_VECTOR f = { lisFor.x, lisFor.y, lisFor.z };
	FMOD_VECTOR u = { lisUp.x,  lisUp.y,  lisUp.z  };
	//Update listener
	m_pSystem->set3DListenerAttributes(0, &p, NULL, &f, &u);

	//update emiiters
	for (int i = 0; i < sfxList.size(); i++)
	{
		sfxList[i]->GetChannel()->set3DAttributes(&sfxList[i]->pos, NULL);
	}
	
	for (auto it = sfxList.begin(); it != sfxList.end();)
	{
		if (!(*it)->IsPlaying())
			it = sfxList.erase(it);
		else
			it++;
	}
}

void SoundSystem::PlaySFX(char* _filename, float _volume, glm::vec3 pos)
{
	SFX* sfx = cachedSFX[_filename];
	if (sfx == nullptr)
	{
		sfx = new SFX(m_pSystem);
		sfx->CreateSound(_filename);
		cachedSFX[_filename] = sfx;
	}

	sfx->PlaySound(pos, _volume);
	sfxList.push_back(sfx);
}

void SoundSystem::PlayBGM(char* _fileName, float _volume)
{
	if (bgm != nullptr)
		if (bgm->IsPlaying())
			bgm->StopSound();
	bgm = cachedBGM[_fileName];
	if (bgm == nullptr)
	{
		bgm = new BGM(m_pSystem);
		bgm->CreateSound(_fileName);
		cachedBGM[_fileName] = bgm;
	}
	
	bgm->PlaySound(_volume);
}
