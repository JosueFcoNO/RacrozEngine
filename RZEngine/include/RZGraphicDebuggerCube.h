#pragma once

namespace rczEngine
{
	class RZ_EXP DebuggerCube : public GraphicDebuggerElement
	{
	public:
		void SetCube(const Vector3& center, const Vector3& scale);
		virtual uint8 GetElementType() { return DEBUG_CUBE; };
	};
};