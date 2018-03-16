#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT AABB
	{
	public:
		AABB() : m_PointMax(INIT_NONE), m_PointMin(INIT_NONE)
		{
			m_PointMin.Set(0,0,0);
			m_PointMax.Set(1,1,1);
		};

		void SetAABB(Vector3 min, Vector3 max);
		void SetAABB(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax);

		void SetAABBc(Vector3 center, Vector3 size);
		void SetAABBc(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

		void AddPoint(Vector3 point);
		void Clear();

		void TransformAndRecalculateAABB(Matrix3 newMatrix);

		Vector3 GetCenter();
		Vector3 GetSize();
		Vector3 GetRadius();

		void SetCenter(Vector3 center);

	private:
		Vector3 m_PointMin, m_PointMax;
	};
}