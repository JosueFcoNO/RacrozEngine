#pragma once
#include <RZVulkanPCH.h>
#include "..\include\RZCamera.h"

namespace rczEngine
{
	void Camera::Init(Vector3 position, Vector3 target, float nearClip, float farClip, float aspectRatio)
	{
		m_Position = position;
		m_Target = target;
		m_Up = Vector3(0, 1, 0);
		m_NearClip = nearClip;
		m_FarClip = farClip;
		m_AspectRatio = aspectRatio;
	}

	void Camera::Move(Vector3 panning)
	{
		m_Position += panning;
		m_Target += panning;
	}

	void Camera::MoveForward(float delta)
	{
		Vector3 fwrd = m_Target - m_Position;
		fwrd.Normalize();
		m_Position += fwrd*delta;
		m_Target += fwrd*delta;
	}

	void Camera::MoveRight(float delta)
	{
		Vector3 fwrd = m_Target - m_Position;
		Vector3 right = m_Up^fwrd;
		right.Normalize();
		
		m_Position += right*delta;
		m_Target   += right*delta;
	}

	void Camera::MoveUp(float delta)
	{
		m_Position += m_Up*delta;
		m_Target += m_Up*delta;
	}

	void Camera::Move(float x, float y, float z)
	{
		Vector3 temp(x,y,z);
		m_Position += temp;
		m_Target += temp;
	}

	void Camera::Rotate(Vector3 vector)
	{
		Vector3 NewVec = (m_Target - m_Position).GetNormalized();
		Vector3 NewVec2 = m_Up.GetNormalized();
		Vector3 NewVec3 = (NewVec^NewVec2).GetNormalized();

		vector /= 100;

		Quaternion AVerQue2(NewVec2, vector.my);
		Quaternion AVerQue3(NewVec3, vector.mx);

		m_Target -= m_Position;
		AVerQue3.Normalize();
		AVerQue2.Normalize();

		Matrix3 temp = AVerQue3.GetAsMatrix3()*AVerQue2.GetAsMatrix3();
		m_Target = temp*m_Target;

		m_Target += m_Position;
	}

	void Camera::RotateComplete(Vector3 vector)
	{
		Vector3 NewVec = (m_Target - m_Position).GetNormalized();
		Vector3 NewVec2 = m_Up.GetNormalized();
		Vector3 NewVec3 = (NewVec^NewVec2).GetNormalized();

		vector /= 100;

		Quaternion AVerQue2(NewVec2, vector.my);
		Quaternion AVerQue3(NewVec3, vector.mx);

		m_Target -= m_Position;
		AVerQue3.Normalize();
		AVerQue2.Normalize();

		Matrix3 temp = AVerQue3.GetAsMatrix3()*AVerQue2.GetAsMatrix3();
		m_Target = temp*m_Target;
		m_Target += m_Position;

		auto m = Matrix4::LookAtMatrixZ(m_Position, Vector3(0,1,0), m_Target);
		m.m_rows[3].Set(0, 0, 0, 1);

		m_Up = m*Vector3(0,1,0);

	}

	void Camera::Orbit(Vector3 vector)
	{
		Vector3 NewVec  = (m_Target - m_Position).GetNormalized();
		Vector3 NewVec2 = m_Up.GetNormalized();
		Vector3 NewVec3 = (NewVec^NewVec2).GetNormalized();

		vector /= 100;

		Quaternion AVerQue2(NewVec2, vector.my);
		Quaternion AVerQue3(NewVec3, vector.mx);

		m_Position -= m_Target;

		AVerQue3.Normalize();
		AVerQue2.Normalize();

		Matrix3 temp = AVerQue3.GetAsMatrix3()*AVerQue2.GetAsMatrix3();
		m_Position = temp*m_Position;
		
		m_Position += m_Target;
	}

	Matrix4 Camera::GetViewMatrix()
	{
		return Matrix4::LookAtMatrix(m_Position, Vector3(0,1,0), m_Target);
	}

	Matrix4 Camera::GetProjMatrix()
	{
		return Matrix4::PerpsProjectedSpace(Math::DegreesToRad(45.0f), m_AspectRatio, m_NearClip, m_FarClip);
	}

	Vector3 Camera::GetViewDir()
	{
		Vector3 Temp = m_Target - m_Position;
		return Temp.GetNormalized();
	}

	Vector3 Camera::GetTarget()
	{
		return m_Target;
	}

	Vector3 Camera::GetPosition()
	{
		return m_Position;
	}

	void Camera::CalculateUp()
	{
		Vector3 Forward = (m_Target - m_Position).GetNormalized();
		Vector3 Up = m_Up.GetNormalized();
		Vector3 Right = (Forward^Up).GetNormalized();

		Quaternion RightRotation(Right, Math::DegreesToRad(-1));

		RightRotation.Normalize();

		Matrix3 temp = RightRotation.GetAsMatrix3();
		m_Up = temp*Forward;
		m_Up.Normalize();

	}
};