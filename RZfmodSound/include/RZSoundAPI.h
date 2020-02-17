#pragma once

namespace rczEngine
{
	class RZ_EXP SoundAPI
	{
	public:
		~SoundAPI();
		void Destroy();
		bool InitSound();

		void Set3DListener(Vector3 pos, Vector3 vel, Vector3 fwrd);

		bool CreateSound(char* filePath, SoundCore& out_Sound);
		bool PlaySnd(SoundCore& sound);

		bool CreateMusic(char* filePath, SoundCore& out_Music);
		bool PlayMusic(SoundCore& music);

	private:
		FMOD::System* m_System = NULL;

	};
}