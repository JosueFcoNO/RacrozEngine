#pragma once
#include <RZUtilityPCH.h>
#include "RZPlane.h"

namespace rczEngine
{
	///////////////////////////////////////////////
	////////Plane
	///////////////////////////////////////////////

	Plane::Plane() : Vector3(INIT_NONE)
	{
		m_x = 0;
		m_y = 1;
		m_z = 0;
		m_w = 0;
	}

	Plane::Plane(float x, float y, float z, float w) : Vector3(INIT_NONE)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}

	Plane::Plane(Vector3 Normal, float w) : Vector3(INIT_NONE)
	{ 
		m_x = Normal.m_x;
		m_y = Normal.m_y;
		m_z = Normal.m_z;
		m_w = w;
	}
}