#pragma once

namespace rczEngine
{
	class RandomZiggurat : public RandomNumberGen
	{
	public:
		virtual uint32 GetRandomNumber();
		virtual float GetRandomNumberN();

	private:
		float TestX(float x);

	};
}