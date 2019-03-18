#pragma once

namespace rczEngine
{
	class RZ_EXP DebuggerLineList : public GraphicDebuggerElement
	{
	public:
		void SetLineList(const Vector<Vector3>& linePoints);
		void SetLineListIndex(const Vector<Vector3>& linePoints, const Vector<uint32>& indexPoints);

		virtual uint8 GetElementType() { return DEBUG_LINE; };
	};
}