#include <RZSoundPCH.h>

namespace rczEngine
{
	void Sound::Play()
	{
		SoundManager::Instance().PlaySound(*this);
	}

	void Sound::Load(char* filePath, bool addToResourceManager)
	{
		SoundManager::Instance().CreateSound(filePath, *this);
	}
}