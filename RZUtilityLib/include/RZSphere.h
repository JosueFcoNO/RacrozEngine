#pragma once

namespace rczEngine
{
	class RZ_EXP Sphere
	{
	public:
		Sphere();
		Sphere(float x, float y, float z, float Radius);

		void Set(float x, float y, float z, float Radius);
		bool CheckSphereCollision(Sphere sph);
		Vector3 GetPosition();
		float GetRadius();

	private:
		Vector3 m_Position;
		float m_Radius;
	};
}