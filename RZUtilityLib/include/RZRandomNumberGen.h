#pragma once

namespace rczEngine
{
	///A Random Number Generator Interface.
	class RZ_UTILITY_EXPORT RandomNumberGen
	{
	public:
		virtual ~RandomNumberGen() {};

		///Returns a random number
		virtual uint32 GetRandomNumber() = 0;

		///Returns a random number between 0 and 1.0f
		virtual float GetRandomNumberN() = 0;

		///Set the set for the random
		FORCEINLINE void SetRandomSeed(uint32 seed) { m_Seed = seed; std::srand(m_Seed); };

	protected:
		///The current Random Seed
		uint32 m_Seed;
	};
}