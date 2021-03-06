#pragma once
#include "CbnUtilitiesPCH.h"

namespace rczEngine
{
	enum class eInit
	{
		Zero,
		Unit,
		None
	};

	class Logger;
	class Profiler;

	class Math; 
	class Matrix3;
	class Matrix4;
	class Plane;
	class Quaternion;
	class Line;
	class Sphere;
	class AABB;

	class Degree; ///Used for homogenous degree use
	class Radian; ///Used for homogenous radian use

	class Vector2; ///A two dimensional vector
	class Vector3; ///A three dimensional vector
	class Vector4; ///A four dimensional vector
	class Vector2I; ///An integer two dimensional vector

	class Rect2Di;
	class Rect2D;
	class Rect3D;

	class Color;

	class Camera;
	class Frustum;
	
};