#pragma once

namespace rczEngine
{
	///A Random Number Generator Interface.
	class RZ_EXP RandomNumberGen
	{
	public:
		RandomNumberGen() = default;
		virtual ~RandomNumberGen() = default;

		///Uncopyable and Unmovable.
		RandomNumberGen(const RandomNumberGen&) = delete;             // copy constructor
		RandomNumberGen& operator=(const RandomNumberGen&) = delete;  // copy assignment
		RandomNumberGen(RandomNumberGen&&) = delete;                  // move constructor
		RandomNumberGen& operator=(RandomNumberGen&&) = delete;       // move assignment

		///Returns a random number
		virtual uint32 GetRandomNumber() noexcept = 0;

		///Returns a random number between 0 and 1.0f
		virtual float GetRandomNumberN() noexcept = 0;

		///Set the set for the random
		FORCEINLINE void SetRandomSeed(uint32 seed) noexcept { m_Seed = seed; std::srand(m_Seed); };

	protected:

		///The current Random Seed
		uint32 m_Seed = 0;
	};
}