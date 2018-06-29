#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT RandomBoxMuller : public RandomNumberGen
	{
	public:
		virtual ~RandomBoxMuller() {};

		///Returns a random number
		virtual uint32 GetRandomNumber()
		{
			return static_cast<uint32>(abs(GetRandomNumberN()) / m_maxValueReturned * UINT32_MAX);
		};

		///Returns a random number between 0 and 1.0f
		virtual float GetRandomNumberN()
		{
			//if (m_randomStored != 0.0f)
			//{
			//	float r = m_randomStored;
			//	m_randomStored = 0.0f;
			//	return r;
			//}

			float x1, x2, w, y1, y2;

			do {
				x1 = 2.0f * (float(rand()) / RAND_MAX) - 1.0f;
				x2 = 2.0f * (float(rand()) / RAND_MAX) - 1.0f;
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