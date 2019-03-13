#pragma once

namespace rczEngine
{
	enum eFrustum
	{
		fNear,
		fFar,
		fLeft,
		fRight,
		fTop,
		fBottom,
		fMax_Sides
	};

	class RZ_EXP Frustum
	{
		public:
			void CalculateFrustum(const Camera& camera);
			Vector4 NearPoints[4];
			Vector4 FarPoints[4];

		private:
			Plane m_Planes[eFrustum::fMax_Sides];


	};
}