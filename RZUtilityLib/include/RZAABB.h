#pragma once

namespace rczEngine
{
	///Axis Aligned Bounding Box 
	class RZ_EXP AABB
	{
	public:
		AABB() noexcept { Clear(); };

		void SetAABB(Vector3 minV, Vector3 maxV) noexcept;
		void SetAABB(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax) noexcept;

		void SetAABBc(Vector3 center, Vector3 size) noexcept;
		void SetAABBc(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize) noexcept;

		void AddPoint(Vector3 point) noexcept;
		void Clear() noexcept;

		Vector<Vector3> GetCorners() const noexcept;

		void TransformAndRecalculateAABB(Matrix3 newMatrix) noexcept;

		FORCEINLINE bool CheckPointCollision(const Vector3& point) const noexcept
		{
			return (point >= m_PointMin && point <= m_PointMax);
		}

		FORCEINLINE Vector3 GetCenter() const noexcept
		{
			return m_PointMin + GetRadius();
		}

		FORCEINLINE Vector3 GetSize() const noexcept
		{
			return (m_PointMax - m_PointMin);
		}

		FORCEINLINE Vector3 GetRadius() const noexcept
		{
			return GetSize() / 2;
		}

		FORCEINLINE void SetCenter(Vector3 center) noexcept 
		{
			m_PointMax += center;
			m_PointMin += center;
		}

		Vector3 m_PointMin, m_PointMax;
	};
}