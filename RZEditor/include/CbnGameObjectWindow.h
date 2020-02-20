#pragma once

namespace rczEngine
{
	class RZ_EXP GameObjectWindow : public BaseWindow
	{
	public:

	protected:
		virtual void Render() override;

		Vector3 m_Position;
		Vector3 m_Orientation;
		Vector3 m_Scale;
	};
};