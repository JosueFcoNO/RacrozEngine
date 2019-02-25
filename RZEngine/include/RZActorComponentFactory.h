#pragma once

namespace rczEngine
{
	class RZ_EXP GameObject;
	class RZ_EXP Component;

	typedef StrPtr<GameObject> StrGameObjectPtr;
	typedef StrPtr<Component> StrCmpPtr;
	typedef WeakPtr<GameObject> WeakGameObjPtr;
	typedef WeakPtr<Component> WeakCmpPtr;
	typedef uint16 ComponentType;
	typedef int32 GameObjectID;
	typedef uint16 ComponentId;

	typedef Map<ComponentType, StrCmpPtr> ComponentMap;
	typedef Map<GameObjectID, StrGameObjectPtr> StrGameObjectMap;
	typedef Map<GameObjectID, WeakGameObjPtr> WeakGameObjectMap;
	typedef Vector<WeakGameObjPtr> GameObjectVector;

	enum { SERIAL_COMPONENT_OFFSET = 1000, SERIAL_RESOURCE_OFFSET = 2000};


	class RZ_EXP ActorComponentFactory
	{
	private:
		static ActorComponentFactory*& _Instance();

	public:
		static void Start();
		static ActorComponentFactory* Pointer();
		static void ShutDown();

		StrGameObjectPtr CreateActor(const String& name = "GameObj", Vector3 position = { 0,0,0 }, Vector3 orientation = { 0,0,0 }, Vector3 scale = { 1,1,1 });
		StrCmpPtr CreateComponent(eCOMPONENT_ID component, WeakGameObjPtr owner);

		FORCEINLINE void Reset() { m_LastId = 0; m_LastCmpID = 0; };
		void SetLastID(ComponentId id) { m_LastId = id; };

	private:
		ComponentId m_LastId = 0;
		ComponentId m_LastCmpID = 0;

		ComponentId GetNewActorId() { return ++m_LastId; };

	};
}