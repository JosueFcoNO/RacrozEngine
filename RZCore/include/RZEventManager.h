#pragma once

namespace rczEngine
{
	typedef List<EventSub*> EventSubscribers;
	typedef UoMap<EVENT_TYPE, EventSubscribers> SubcriptionMap;

	class RZ_UTILITY_EXPORT EventManager
	{
	private:
		static EventManager*& _Instance();

	public:
		static void Start();
		static EventManager* Pointer();
		static void ShutDown();

		void InitEventManager();
		void CheckEvents();
		void SubscribeToEvent(IEventHandler* subscriber, EVENT_TYPE eType, int32 parameterToSub);
		void FlushKeyBuffer() { m_KeyBuffer.clear(); };
		char* GetKeyBuffer() { return &m_KeyBuffer[0]; };
		void SetKeyBuffer(char* str);

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