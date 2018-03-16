#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Plane : public Vector3
	{
	public:
		Plane();
		Plane(float x, float y, float z, float w);
		Plane(Vector3 Normal, float w);

		float m_w;
	};
}