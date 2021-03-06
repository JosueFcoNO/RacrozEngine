#pragma once
#include <optional>

namespace rczEngine
{
	class RZ_EXP Scene
	{
	public:
		///Calls Destroy.
		~Scene()
		{
			Destroy();
		}; 

		void InitScene(const String&& name);
		void ClearScene();
		void Update(float deltaTime);
		void Destroy();

		void SaveScene();
		void LoadScene();

		WeakGameObjPtr CreateActor(const String&& name = "GameObj", GameObject* parent = NULL, Vector3 position = { 0,0,0 }, Vector3 orientation = { 0,0,0 }, Vector3 scale = { 1,1,1 });
		StrCmpPtr CreateComponent(eComponentID type, GameObjectID owner);
		StrCmpPtr CreateComponent(eComponentID type, StrGameObjectPtr owner);

		WeakGameObjPtr FindActor(GameObjectID id);
		WeakGameObjPtr FindActor(const String& name);

		void AddRootNodeChild(StrGameObjectPtr node);
		void RemoveGameObject(GameObjectID actorId);

		void AddActor(StrGameObjectPtr node);
		Vector<WeakPtr<GameObject>> FindActorsWithComponent(eComponentID cmp);

		Gfx::ConstantBuffer m_WorldMatrix;

		Gfx::GfxCore* m_gfx;
		ResVault* m_res;
		LightManager* m_light;

		StrGameObjectMap m_SceneActorMap;
		StrGameObjectPtr m_RootNode;

		//Octree sceneOctree;

	private:
		String m_Name;

		//Scenes Json .cbnScene
		//Textures Compressed Binary .cbnTexture
		//Audios Compressed Binary .cbnAudio
		//Fonts Font Binary .cbnFont
		//Prefabs/Blueprints Json .cbnPrefab
		//Materials Json .cbnMaterial

		
		Vector<StrPtr<GameObject>> m_GameObjectsToDelete;
	};

}