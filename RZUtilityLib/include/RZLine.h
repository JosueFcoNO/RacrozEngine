#pragma once

namespace rczEngine
{
	///Line class
	class RZ_EXP Line
	{
	public:
		Line() noexcept : m_PointOne(eInit::None), m_PointTwo(eInit::None) {};
		Line(Vector3 p1, Vector3 p2) noexcept : m_PointOne(p1), m_PointTwo(p2) {};
		Line(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z) noexcept : m_PointOne(p1x, p1y, p1z), m_PointTwo(p2x, p2y, p2z) {};
		
		bool TestPoint(const Vector3& point);
		bool IntersectionSphere(const Sphere& sphere);

		union
		{
			struct
			{
				Vector3 m_PointOne;
				Vector3 m_PointTwo;
			};

			Vector3 m_PointsArray[2];

			float m_ComponentArray[6];
		};
	};
}