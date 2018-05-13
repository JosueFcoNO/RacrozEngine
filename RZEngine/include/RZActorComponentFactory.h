#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT GameObject;
	class RZ_UTILITY_EXPORT Component;

	typedef StrPtr<GameObject> StrGameObjectPtr;
	typedef StrPtr<Component> StrCmpPtr;
	typedef WeakPtr<GameObject> WeakGameObjectPtr;
	typedef WeakPtr<Component> WeakCmpPtr;
	typedef uint16 ComponentType;
	typedef int32 GameObjectID;

	typedef Map<ComponentType, StrCmpPtr> ComponentMap;
	typedef Map<GameObjectID, StrGameObjectPtr> StrGameObjectMap;
	typedef Map<GameObjectID, WeakGameObjectPtr> WeakGameObjectMap;
	typedef Vector<WeakGameObjectPtr> GameObjectVector;

	enum { SERIAL_COMPONENT_OFFSET = 1000, SERIAL_RESOURCE_OFFSET = 2000};


	class RZ_UTILITY_EXPORT ActorComponentFactory
	{
	private:
		static ActorComponentFactory*& _Instance();

	public:
		static void Start();
		static ActorComponentFactory* Pointer();
		static void ShutDown();

		StrGameObjectPtr CreateActor(const char* name = "GameObj", Vector3 position = { 0,0,0 }, Vector3 orientation = { 0,0,0 }, Vector3 scale = { 1,1,1 });
		StrCmpPtr CreateComponent(eCOMPONENT_ID component, WeakGameObjectPtr owner);

		FORCEINLINE void Reset() { m_LastId = 0; m_LastCmpID = 0; };
		void SetLastID(uint32 id) { m_LastId = id; };

	private:
		uint32 m_LastId = 0;
		uint32 m_LastCmpID = 0;

		uint32 GetNewActorId() { return ++m_LastId; };

	};
}