#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT KeyFrame
	{
	public:
		KeyFrame() : m_Scale(INIT_ZERO), m_Position(INIT_ZERO), m_Rotation(INIT_ZERO)
		{
		}

		Vector3 m_Scale;
		Vector3 m_Position;
		Quaternion m_Rotation;
		float m_Time = -1;
	};
}