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
			void CalculateFrustum(const Matrix4& viewMat, const Matrix4 & projMatrix, const Matrix4 & camTransform);
			Vector3 NearPoints[4];
			Vector3 FarPoints[4];

		private:
			Plane m_Planes[eFrustum::fMax_Sides];


	};
}