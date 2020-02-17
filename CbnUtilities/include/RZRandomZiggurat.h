#pragma once

namespace rczEngine
{
	class RandomZiggurat : public RandomNumberGen
	{
	public:
		uint32 GetRandomNumber() noexcept override;
		float GetRandomNumberN() noexcept override;

	private:
		float TestX(float x) noexcept;

	};
}