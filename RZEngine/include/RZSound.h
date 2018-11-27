#pragma once

namespace rczEngine
{
	class RZ_EXP Sound : public Resource
	{
	public:
		Sound() {};
		void Play();

		virtual void* ptr() { return this; };
		virtual void Load(char* filePath, bool addToResourceManager = true);

		SoundCore m_SoundCore;
	};
}
