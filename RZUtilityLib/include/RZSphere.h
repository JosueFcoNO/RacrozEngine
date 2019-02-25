#pragma once

namespace rczEngine
{
	class Sphere
	{
	public:
		RZ_EXP Sphere() noexcept : m_Position(eInit::Zero)
		{
			m_Radius = 1.0f;
		}

		RZ_EXP Sphere(float x, float y, float z, float Radius) noexcept :
			m_Position(x, y, z), 
			m_Radius(Radius) {};

		RZ_EXP FORCEINLINE void Set(float x, float y, float z, float Radius) noexcept;
		RZ_EXP FORCEINLINE bool CheckSphereCollision(Sphere sph) const noexcept;
		RZ_EXP FORCEINLINE Vector3 GetPosition() const noexcept;
		RZ_EXP FORCEINLINE float GetRadius() const noexcept;

	private:
		Vector3 m_Position;
		float m_Radius;
	};
}