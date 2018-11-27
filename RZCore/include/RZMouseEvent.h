#pragma once
#include <RZCorePCH.h>

namespace rczEngine
{
	typedef std::function<void (int32, int32, int32)> MouseCallback;

	class RZ_EXP IMouseHandler : public IEventHandler
	{
	public:
		virtual void OnClick(int32 mb = 0, int32 mx = 0, int32 my = 0) = 0;
		virtual void OnHold(int32 mb = 0, int32 mx = 0, int32 my = 0) = 0;
		virtual void OnReleased(int32 mb = 0, int32 mx = 0, int32 my = 0) = 0;
		virtual void OnMoved(int32 mb = 0, int32 mx = 0, int32 my = 0) = 0;
		EVENT_HANDLER GetEventHandlerType() { return EVENT_HANDLER::HANDLER_MOUSE; };

	};

	class MouseSub : public EventSub
	{
	public:
		virtual EVENT_HANDLER GetEventHandler() { return EVENT_HANDLER::HANDLER_MOUSE; };

		void CheckAndCallbackMouse(int32 mouseButton, int32 mouseX, int32 mouseY)
		{
			if (m_Mouse == mouseButton)
			{
				m_Callback(mouseButton, mouseX, mouseY);
			};
		};

		MouseCallback m_Callback;
		int32 m_Mouse;
	};
};