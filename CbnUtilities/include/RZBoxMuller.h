#pragma once

namespace rczEngine
{
	class RZ_EXP RandomBoxMuller : public RandomNumberGen
	{
	public:
		///Returns a random number
		uint32 GetRandomNumber() noexcept override
		{
			return static_cast<uint32>(abs(GetRandomNumberN()) / m_maxValueReturned * UINT32_MAX);
		};

		///Returns a random number between 0 and 1.0f
		float GetRandomNumberN() noexcept override
		{
			//if (m_randomStored != 0.0f)
			//{
			//	float r = m_randomStored;
			//	m_randomStored = 0.0f;
			//	return r;
			//}

			auto x1 = 0.0f;
			auto x2 = 0.0f;
			auto w = 0.0f;
			auto y1 = 0.0f;
			auto y2 = 0.0f;

			do {
				x1 = 2.0f * (gsl::narrow_cast<float>(rand()) / RAND_MAX) - 1.0f;
				x2 = 2.0f * (gsl::narrow_cast<float>(rand()) / RAND_MAX) - 1.0f;
				w = x1 * x1 + x2 * x2;
			} while (w >= 1.0);

			w = sqrtf((-2.0f * logf(w)) / w);
			y1 = x1 * w;
			m_randomStored = y2 = x2 * w;

			return abs(y1) / m_maxValueReturned;
		};

	private:
		float m_randomStored = 0.0f;
		const float m_maxValueReturned = 3.0f;
	};
}