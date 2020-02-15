#pragma once

namespace rczEngine
{
	///Line class
	class RZ_EXP Line2D
	{
	public:
		Line2D() noexcept : m_PointOne(eInit::None), m_PointTwo(eInit::None) {};
		Line2D(Vector3 p1, Vector3 p2) noexcept : m_PointOne(p1), m_PointTwo(p2) {};
		Line2D(Vector2 p1, Vector2 p2) noexcept : m_PointOne(p1), m_PointTwo(p2) {};
		Line2D(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z) noexcept : m_PointOne(p1x, p1y, p1z), m_PointTwo(p2x, p2y, p2z) {};

		const float Slope();
		const float XIntercept();
		const float YIntercept();

		bool TestPoint(const Vector2& point);
		bool IntersectionSphere(const Sphere& sphere);

		union
		{
			struct
			{
				Vector2 m_PointOne;
				Vector2 m_PointTwo;
			};

			Vector2 m_PointsArray[2];

			float m_ComponentArray[4];
		};

	};
}