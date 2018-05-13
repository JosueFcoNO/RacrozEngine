#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT KeyFrame : public Serializable
	{
	public:
		KeyFrame() : m_Scale(INIT_ZERO), m_Position(INIT_ZERO), m_Rotation(INIT_ZERO)
		{
		}

		virtual void Serialize()
		{
			//Set the serial for this object.
			auto ser = Serializer::Pointer();
			ser->SetNextObjectSerial(SERIAL_KEYFRAME);

			//Write the keyframe time.
			ser->WriteData(&m_Time, sizeof(float));

			//Write the position vector
			ser->WriteData(&m_Position, sizeof(m_Position));
			//Write the scale.
			ser->WriteData(&m_Scale, sizeof(m_Scale));
			//Write the rotation.
			ser->WriteData(&m_Rotation, sizeof(m_Rotation));
		};

		virtual void DeSerialize()
		{
			//Set the serial for this object.
			auto ser = Serializer::Pointer();

			//Read the keyframe time.
			ser->ReadData(&m_Time, sizeof(float));

			//Read the position vector
			ser->ReadData(&m_Position, sizeof(m_Position));
			//Read the scale.
			ser->ReadData(&m_Scale, sizeof(m_Scale));
			//Read the rotation.
			ser->ReadData(&m_Rotation, sizeof(m_Rotation));
		};

		Vector3 m_Scale;
		Vector3 m_Position;
		Quaternion m_Rotation;
		float m_Time = -1;
	};
}