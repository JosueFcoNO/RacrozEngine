#pragma once

namespace rczEngine
{
	typedef List<EventSub*> EventSubscribers;
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
		RZ_EXP void FlushKeyBuffer() { m_KeyBuffer.clear(); };
		RZ_EXP char* GetKeyBuffer() { return &m_KeyBuffer[0]; };
		RZ_EXP void SetKeyBuffer(char* str);

	private:
		void AddNewKeySub(EVENT_TYPE eType, KeyCallback& f, int32 key);
		void AddNewMouseSub(EVENT_TYPE eType, MouseCallback& f, int32 mouseButton);
		void CheckKeyboardEvents();
		void CheckMouseEvents();
		void AddKeyStrokeToBuffer(KEY key);

		SubcriptionMap m_EventMap;
		byte Keyboard[256];
		MouseData m_Mouse;
		String m_KeyBuffer;

		bool m_Mayus = false;
	};
}