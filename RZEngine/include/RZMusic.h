#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Music : public Resource
	{
	public:
		void Play();

		virtual void* ptr() { return this; };
		virtual void Load(char* filePath, bool addToResourceManager = true);

		SoundCore m_MusicCore;
	};
}