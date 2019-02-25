#include <RZCorePCH.h>
#include "..\include\RZEventManager.h"

namespace rczEngine
{
	EventManager*& EventManager::_Instance()
	{
		static EventManager* instance = nullptr;
		return instance;
	}

	void EventManager::Start()
	{
		(_Instance()) = new EventManager;
	}

	EventManager* EventManager::Pointer()
	{
		return _Instance();
	}

	void EventManager::ShutDown()
	{
		delete _Instance();
	}

	void EventManager::InitEventManager()
	{
		m_KeyBuffer.clear();
	}

	void EventManager::CheckEvents()
	{
		CheckKeyboardEvents();
		CheckMouseEvents();
	}

	void EventManager::SubscribeToEvent(IEventHandler * subscriber, EVENT_TYPE eType, int32 parameterToSub)
	{
		auto handlerType = subscriber->GetEventHandlerType();

		if (handlerType == EVENT_HANDLER::HANDLER_KEY)
		{
			KeyCallback keyFunc;

			switch (eType)
			{
			default:
			case EVENT_TYPE::EVENT_KEY_DOWN:
				keyFunc = std::bind(&IKeyHandler::OnKeyDown, ((IKeyHandler*)(subscriber)), std::placeholders::_1);
				break;
			case EVENT_TYPE::EVENT_KEY_PRESSED:
				keyFunc = std::bind(&IKeyHandler::OnKeyPressed, ((IKeyHandler*)(subscriber)), std::placeholders::_1);
				break;
			case EVENT_TYPE::EVENT_KEY_UP:
				keyFunc = std::bind(&IKeyHandler::OnKeyUp, ((IKeyHandler*)(subscriber)), std::placeholders::_1);
				break;
			}

			AddNewKeySub(eType, keyFunc, parameterToSub);
		}
		else
			if (handlerType == EVENT_HANDLER::HANDLER_MOUSE)
			{
				MouseCallback mouseFunc;

				switch (eType)
				{
				default:
				case EVENT_TYPE::EVENT_MOUSE_DOWN:
					mouseFunc = std::bind(&IMouseHandler::OnClick, ((IMouseHandler*)(subscriber)), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
					break;
				case EVENT_TYPE::EVENT_MOUSE_UP:
					mouseFunc = std::bind(&IMouseHandler::OnReleased, ((IMouseHandler*)(subscriber)), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
					break;
				case EVENT_TYPE::EVENT_MOUSE_PRESSED:
					mouseFunc = std::bind(&IMouseHandler::OnHold, ((IMouseHandler*)(subscriber)), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
					break;
				case EVENT_TYPE::EVENT_MOUSE_MOVED:
					mouseFunc = std::bind(&IMouseHandler::OnMoved, ((IMouseHandler*)(subscriber)), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
					break;
				}

				AddNewMouseSub(eType, mouseFunc, parameterToSub);
			}
	}

	void EventManager::SetKeyBuffer(char * str)
	{
		m_KeyBuffer.clear();
		m_KeyBuffer += str;
	}

	void EventManager::AddNewKeySub(EVENT_TYPE eType, KeyCallback& f, int32 key)
	{
		KeySub* newEvent = new KeySub;
		newEvent->m_Callback = f;
		newEvent->eKey = key;

		m_EventMap[eType].push_front(newEvent);
	}

	void EventManager::AddNewMouseSub(EVENT_TYPE eType, MouseCallback & f, int32 mouseButton)
	{
		MouseSub* newEvent = new MouseSub;
		newEvent->m_Callback = f;
		newEvent->m_Mouse = mouseButton;

		m_EventMap[eType].push_front(newEvent);
	}

	void EventManager::CheckKeyboardEvents()
	{
		///Create a new keyboard state
		byte newKeyBoard[256];

		///Save the current keyboard state
		Input::Pointer()->GetKeyboard(&newKeyBoard[0]);

		///Iterate through the keys
		for (int32 i = 1; i < 256; ++i)
		{
			///Create a pointer to the list of subscribers
			EventSubscribers* SubscriberList = nullptr;

			//PRESSED
			if ((newKeyBoard[i] & 0x80) && (Keyboard[i] & 0x80))
			{
				SubscriberList = &m_EventMap[EVENT_TYPE::EVENT_KEY_PRESSED];
			}

			//KEY UP
			if (!(newKeyBoard[i] & 0x80) && (Keyboard[i] & 0x80))
			{
				SubscriberList = &m_EventMap[EVENT_TYPE::EVENT_KEY_UP];

				///If key up was shift, deactivate mayus
				if (i == KEY_LSHIFT || i == KEY_RSHIFT)
					m_Mayus = false;
			}

			//KEY DOWN
			if ((newKeyBoard[i] & 0x80) && !(Keyboard[i] & 0x80))
			{
				SubscriberList = &m_EventMap[EVENT_TYPE::EVENT_KEY_DOWN];

				///If key down was shift or mayus, activate mayus
				if (i == KEY_LSHIFT || i == KEY_RSHIFT)
					m_Mayus = true;

				if (i == KEY_CAPITAL)
				{
					m_Mayus = !m_Mayus;
				}

				AddKeyStrokeToBuffer((KEY)i);
			}

			if (SubscriberList == nullptr)
			{
				continue;
			}

			///Go through the subscribers of that event and call them
			for (auto it = SubscriberList->begin(); it != SubscriberList->end(); ++it)
			{
				KeySub* ptr = (KeySub*)(*it);
				ptr->CheckAndCallBackKey(i);
			}
		}

		///Copy the new state to the old keyboard state
		Input::Pointer()->GetKeyboard(Keyboard);

	}

	void EventManager::CheckMouseEvents()
	{
		Input::Pointer()->UpdateMouseData();
		auto mouseData = Input::Pointer()->GetMouseData();

		if ((mouseData.buttons[MOUSE_BUTTON::MOUSE_LEFT] & 0x80) && !(m_Mouse.buttons[MOUSE_LEFT] & 0x80)) //CLICKED
		{
			for (auto it = m_EventMap[EVENT_MOUSE_DOWN].begin(); it != m_EventMap[EVENT_MOUSE_DOWN].end(); ++it)
			{
				auto ptr = (MouseSub*)(*it);

				ptr->CheckAndCallbackMouse(MOUSE_LEFT, (int32)mouseData.x, (int32)mouseData.y);
			}
		}

		if (!(mouseData.buttons[MOUSE_LEFT] & 0x80) && (m_Mouse.buttons[MOUSE_LEFT] & 0x80)) //RELEASED
		{
			for (auto it = m_EventMap[EVENT_MOUSE_UP].begin(); it != m_EventMap[EVENT_MOUSE_UP].end(); ++it)
			{
				auto ptr = (MouseSub*)(*it);

				ptr->CheckAndCallbackMouse(MOUSE_LEFT, (int32)mouseData.x, (int32)mouseData.y);
			}
		}

		if ((mouseData.buttons[MOUSE_LEFT] & 0x80) && (m_Mouse.buttons[MOUSE_LEFT] & 0x80)) //HOLD
		{
			for (auto it = m_EventMap[EVENT_MOUSE_PRESSED].begin(); it != m_EventMap[EVENT_MOUSE_PRESSED].end(); ++it)
			{
				auto ptr = (MouseSub*)(*it);

				ptr->CheckAndCallbackMouse(MOUSE_LEFT, (int32)mouseData.x, (int32)mouseData.y);
			}
		}

		if (mouseData.x != m_Mouse.x || mouseData.y != m_Mouse.y)
		{
			for (auto it = m_EventMap[EVENT_MOUSE_MOVED].begin(); it != m_EventMap[EVENT_MOUSE_MOVED].end(); ++it)
			{
				auto ptr = (MouseSub*)(*it);

				ptr->CheckAndCallbackMouse(-1, (int32)mouseData.x, (int32)mouseData.y);
			}
		}

		m_Mouse = mouseData;

	}

	void EventManager::AddKeyStrokeToBuffer(KEY key)
	{
		///If the key is a digit from 1 to 9
		if (key >= KEY_1 && key <= KEY_9)
		{
			m_KeyBuffer += (char)key + (char)47;
			return;
		}

		if (key == KEY_0)
		{
			m_KeyBuffer += '0';
			return;
		}

		switch (key)
		{
		case KEY_Q: m_KeyBuffer += m_Mayus ? 'Q' : 'q'; return;
		case KEY_W: m_KeyBuffer += m_Mayus ? 'W' : 'w'; return;
		case KEY_E: m_KeyBuffer += m_Mayus ? 'E' : 'e'; return;
		case KEY_R: m_KeyBuffer += m_Mayus ? 'R' : 'r'; return;
		case KEY_T: m_KeyBuffer += m_Mayus ? 'T' : 't'; return;
		case KEY_Y: m_KeyBuffer += m_Mayus ? 'Y' : 'y'; return;
		case KEY_U: m_KeyBuffer += m_Mayus ? 'U' : 'u'; return;
		case KEY_I: m_KeyBuffer += m_Mayus ? 'I' : 'i'; return;
		case KEY_O: m_KeyBuffer += m_Mayus ? 'O' : 'o'; return;
		case KEY_P: m_KeyBuffer += m_Mayus ? 'P' : 'p'; return;
		case KEY_A: m_KeyBuffer += m_Mayus ? 'A' : 'a'; return;
		case KEY_S: m_KeyBuffer += m_Mayus ? 'S' : 's'; return;
		case KEY_D: m_KeyBuffer += m_Mayus ? 'D' : 'd'; return;
		case KEY_F: m_KeyBuffer += m_Mayus ? 'F' : 'f'; return;
		case KEY_G: m_KeyBuffer += m_Mayus ? 'G' : 'g'; return;
		case KEY_H: m_KeyBuffer += m_Mayus ? 'H' : 'h'; return;
		case KEY_J: m_KeyBuffer += m_Mayus ? 'J' : 'j'; return;
		case KEY_K: m_KeyBuffer += m_Mayus ? 'K' : 'k'; return;
		case KEY_L: m_KeyBuffer += m_Mayus ? 'L' : 'l'; return;
		case KEY_Z: m_KeyBuffer += m_Mayus ? 'Z' : 'z'; return;
		case KEY_X: m_KeyBuffer += m_Mayus ? 'X' : 'x'; return;
		case KEY_C: m_KeyBuffer += m_Mayus ? 'C' : 'c'; return;
		case KEY_V: m_KeyBuffer += m_Mayus ? 'V' : 'v'; return;
		case KEY_B: m_KeyBuffer += m_Mayus ? 'B' : 'b'; return;
		case KEY_N: m_KeyBuffer += m_Mayus ? 'N' : 'n'; return;
		case KEY_M: m_KeyBuffer += m_Mayus ? 'M' : 'm'; return;
		case KEY_PERIOD : m_KeyBuffer += '.'; return;
		case KEY_COMMA: m_KeyBuffer += ','; return;
		case KEY_SPACE: m_KeyBuffer += ' '; return;
		case KEY_BACK: if (m_KeyBuffer.size()) m_KeyBuffer.pop_back(); return;
		}
	}

}