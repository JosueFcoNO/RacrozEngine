#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Triangle 
	{
	public:
		Triangle();
		Triangle(Vector3 p1, Vector3 p2, Vector3 p3);
		void Set(Vector3 p1, Vector3 p2, Vector3 p3);

	private:
		Vector3 P1, P2, P3;
	};
};