#pragma once
#include <RZCorePCH.h>

namespace rczEngine
{
	typedef std::function<void(int32)> KeyCallback;

	class RZ_EXP IKeyHandler : public IEventHandler
	{
	public:
		virtual void OnKeyDown(int32 k) = 0;
		virtual void OnKeyPressed(int32 k) = 0;
		virtual void OnKeyUp(int32 k) = 0;
		EVENT_HANDLER GetEventHandlerType() { return EVENT_HANDLER::HANDLER_KEY; };

	};

	class KeySub : public EventSub
	{
	public:
		virtual EVENT_HANDLER GetEventHandler() { return EVENT_HANDLER::HANDLER_KEY; };

		void CheckAndCallBackKey(int32 parameter)
		{
			if (parameter == eKey)
			{
				m_Callback(eKey);
			}
		};

		KeyCallback m_Callback;
		int32 eKey;
	};
}