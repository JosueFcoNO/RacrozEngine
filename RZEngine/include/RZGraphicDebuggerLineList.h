#pragma once

namespace rczEngine
{
	class RZ_EXP DebuggerLineList : public GraphicDebuggerElement
	{
	public:
		void SetLineList(const Vector<Vector3>& linePoints);
		virtual uint8 GetElementType() { return DEBUG_LINE; };
	};
}