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


	class RZ_UTILITY_EXPORT ActorComponentFactory
	{
	public:
		StrGameObjectPtr CreateActor(char* name = "GameObj", Vector3 position = { 0,0,0 }, Vector3 orientation = { 0,0,0 }, Vector3 scale = { 1,1,1 });
		StrCmpPtr CreateComponent(GameObjectID owner);

	private:
		uint32 m_LastId = 0;

		uint32 GetNewActorId() { return ++m_LastId; };

	};
}