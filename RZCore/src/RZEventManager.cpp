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
	}

	void EventManager::SubscribeToEvent(IEventHandler * subscriber, EVENT_TYPE eType, int32 parameterToSub)
	{
		//keyFunc = std::bind(&IKeyHandler::OnKeyDown, ((IKeyHandler*)(subscriber)), std::placeholders::_1);
	}
}