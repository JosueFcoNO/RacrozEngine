#include <RZSoundPCH.h>

namespace rczEngine
{
	SoundAPI::~SoundAPI()
	{
		Destroy();
	}

	void SoundAPI::Destroy()
	{
		m_System->release();
	}

	bool SoundAPI::InitSound()
	{
		FMOD_RESULT result;
		result = FMOD::System_Create(&m_System);      // Create the main system object.
		if (result != FMOD_OK)
		{
			return false;
		}

		result = m_System->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
		if (result != FMOD_OK)
		{
			return false;
		}

		return true;
	}

	void SoundAPI::Set3DListener(Vector3 pos, Vector3 vel, Vector3 fwrd)
	{
		FMOD_VECTOR position;
		position.x = pos.m_x;
		position.y = pos.m_y;
		position.z = pos.m_z;

		FMOD_VECTOR velocity;
		velocity.x = vel.m_x;
		velocity.y = vel.m_y;
		velocity.z = vel.m_z;

		FMOD_VECTOR forward;
		forward.x = fwrd.m_x;
		forward.y = fwrd.m_y;
		forward.z = fwrd.m_z;

		FMOD_VECTOR up;
		up.x = 0;
		up.y = 1;
		up.z = 0;

		m_System->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
	}

	bool SoundAPI::CreateSound(char* filePath, SoundCore& out_Sound)
	{
		auto result = m_System->createSound(filePath, FMOD_DEFAULT, NULL, &out_Sound.m_Sound);

		if (result != FMOD_OK)
		{
			return false;
		}

		return true;
	}

	bool SoundAPI::PlaySnd(SoundCore & sound)
	{
		auto result = m_System->playSound(sound.m_Sound, sound.m_ChannelSettings, false, NULL);

		if (result != FMOD_OK)
		{
			return false;
		}

		return true;
	}

	bool SoundAPI::CreateMusic(char * filePath, SoundCore & out_Music)
	{
		auto result = m_System->createStream(filePath, FMOD_DEFAULT, NULL, &out_Music.m_Sound);

		if (result != FMOD_OK)
		{
			return false;
		}

		return true;
	}

	bool SoundAPI::PlayMusic(SoundCore & music)
	{
		auto result = m_System->playSound(music.m_Sound, music.m_ChannelSettings, false, NULL);

		if (result != FMOD_OK)
		{
			return false;
		}

		return true;
	}

	
}