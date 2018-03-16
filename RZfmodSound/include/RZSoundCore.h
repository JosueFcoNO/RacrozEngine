#pragma once


namespace rczEngine
{
	class RZ_UTILITY_EXPORT SoundCore
	{
	public:
		FMOD::Sound* m_Sound;
		FMOD::ChannelGroup* m_ChannelSettings;

		void SetPosition(Vector3 pos)
		{
			m_ChannelSettings;
		}
	};

}