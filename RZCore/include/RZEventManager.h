#pragma once

namespace rczEngine
{
	typedef List<StrPtr<EventSub>> EventSubscribers;
	typedef UoMap<EVENT_TYPE, EventSubscribers> SubcriptionMap;

	class EventManager
	{
	private:
		static EventManager*& _Instance();

	public:
		RZ_EXP static void Start();
		RZ_EXP static EventManager* Pointer();
		RZ_EXP static void ShutDown();

		RZ_EXP void InitEventManager();
		RZ_EXP void CheckEvents();
		RZ_EXP void SubscribeToEvent(IEventHandler* subscriber, EVENT_TYPE eType, int32 parameterToSub);
		RZ_EXP void FlushKeyBuffer() noexcept { m_KeyBuffer.clear(); };
		RZ_EXP char* GetKeyBuffer() { return &m_KeyBuffer[0]; };

	private:

		SubcriptionMap m_EventMap;
		std::array<byte, 256> Keyboard;
		MouseData m_Mouse;
		String m_KeyBuffer;

		bool m_Mayus = false;
	};
}