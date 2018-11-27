#pragma once

namespace rczEngine
{
	enum { INVALID_ID = -1 };

	///The class GameObject from the Scene Graph, it's also the actor, has a transform and components.
	class RZ_EXP GameObject : public std::enable_shared_from_this<GameObject>, public Serializable
	{
	public:
		///Calls Destroy()
		~GameObject() {  };

		///The Node constructor takes an id and a name. There can't be a game object without a name.
		GameObject(GameObjectID actorID = INVALID_ID, String actorName = "GameObj") : m_Position(INIT_NONE), m_Orientation(INIT_NONE), m_Scale(INIT_NONE), m_ToLocal(INIT_NONE)
		{
			m_GameObjectID = actorID;
			m_Name = actorName;
		};

		///Inits the node's components
		void Init();
		///Updates the node
		void Update(Scene* scene, float deltaTime);

		///Destroys the GameObject's data
		void Destroy(bool DestroyParentRef = false);

		///Sets the pipeline before rendering
		void PreRender(Scene* scene);
		///Renders the gameobject
		void Render(Scene* scene, ComponentType cmpType, MATERIAL_TYPE mat = MAT_ANY);

		FORCEINLINE void SetParent(StrGameObjectPtr parent) { m_ParentNode = parent; m_ParentID = m_ParentNode.lock()->GetID(); };
		///Returns the parent of the node
		FORCEINLINE WeakGameObjectPtr GetParent() { return m_ParentNode; };
		///Returns a pointer to the GameObjectVector Containing the children of this node
		FORCEINLINE GameObjectVector GetChildren() { return m_ChildrenVector; };

		///Adds a new node child
		void AddChild(WeakGameObjectPtr babyNode)
		{
			m_ChildrenVector.push_back(babyNode);
			m_ChildrenIDs.push_back(babyNode.lock()->GetID());
			babyNode.lock()->SetParent(shared_from_this());
		};
		///Removes a child node
		void RemoveChild(GameObjectID orphanActorId)
		{
			for (int32 i = 0; i < m_ChildrenVector.size(); ++i)
			{
				if (m_ChildrenVector[i].lock()->GetID() == orphanActorId)
				{
					m_ChildrenVector.erase(m_ChildrenVector.begin() + i);
					m_ChildrenIDs.erase(m_ChildrenIDs.begin() + i);
				}
			}
		};

		///Removes a child node
		void RemoveChildren()
		{
			for (int32 i = 0; i < m_ChildrenVector.size(); ++i)
			{
				m_ChildrenVector[i].lock()->RemoveChildren();

				m_ChildrenVector.erase(m_ChildrenVector.begin() + i);
				m_ChildrenIDs.erase(m_ChildrenIDs.begin() + i);
			}
		};

		///Returns the node's id
		FORCEINLINE const GameObjectID& GetID() { return m_GameObjectID; };
		///Returns the node's name
		FORCEINLINE const String& GetName() { return m_Name; };
		///Changes the node's name
		FORCEINLINE void SetName(char* name) { m_Name = name; };

		///Translates the node
		FORCEINLINE void Translate(Vector3 position) { m_DirtyLocalMatrix = true; m_Position += position; };
		///Rotates the node
		FORCEINLINE void Rotate(Vector3 orientation) { m_DirtyLocalMatrix = true; m_Orientation += orientation; };
		///Scales the node
		FORCEINLINE void Scale(Vector3 scale) { m_DirtyLocalMatrix = true; m_Scale += scale; };

		///Sets the position of the Node
		FORCEINLINE void SetPosition(float x, float y, float z) { m_DirtyLocalMatrix = true; m_Position.Set(x, y, z); };
		///Sets the position of the Node
		FORCEINLINE void SetPosition(Vector3 newPos) { m_DirtyLocalMatrix = true; m_Position = newPos; };

		///Sets the orientation of the Node
		FORCEINLINE void SetOrientation(float x, float y, float z) { m_DirtyLocalMatrix = true; m_Orientation = Vector3(x, y, z); };
		///Sets the orientation of the Node
		FORCEINLINE void SetOrientation(Vector3 newOrientation) { m_DirtyLocalMatrix = true; m_Orientation = newOrientation; };

		///Sets the scale of the Node
		FORCEINLINE void SetScale(float x, float y, float z) { m_DirtyLocalMatrix = true; m_Scale = Vector3(x, y, z); };
		///Sets the scale of the Node
		FORCEINLINE void SetScale(Vector3 newScale) { m_DirtyLocalMatrix = true; m_Scale = newScale; };

		///Returns the position of the Node.
		FORCEINLINE Vector3 GetPosition() { return m_Position; };
		///Returns the world position.
		FORCEINLINE Vector3 GetAccumulatedPosition() { return m_ToWorld[0] * m_Position; };
		///Returns the orientation of the node.
		FORCEINLINE Vector3 GetOrientation() { return m_Orientation; };
		///Returns the node's scale.
		FORCEINLINE Vector3 GetScale() { return m_Scale; };
		///Returns the WorldMatrix for this node.
		FORCEINLINE Matrix4 GetToWorldMatrix() { return m_ToWorld[0]; };
		FORCEINLINE Matrix4 GetLocalMatrix();

		void UpdateWorldMatrix();

		///Adds a component of ComponenType to the node
		WeakCmpPtr AddComponent(eCOMPONENT_ID cmp, StrCmpPtr ptr);

		///Returns a strPtr to a component given its id.
		template <class CType>
		WeakPtr<CType> GetComponent(eCOMPONENT_ID cmp)
		{
			StrPtr<CType> ret;

			auto It = m_Components.find(short(cmp));

			if (It != m_Components.end())
			{
				ret = std::dynamic_pointer_cast<CType, Component>(It->second);
			}
			return ret;
		}

		///Returns a component in this gameobject.
		WeakCmpPtr GetComponent(ComponentType cmpId)
		{
			WeakCmpPtr ret;
			auto It = m_Components.find(cmpId);
			if (It != m_Components.end())
			{
				ret = It->second;
			}
			return ret;
		}

		///The handle for the node's material
		ResourceHandle m_Material = NULL;

#ifndef RZ_EDITOR
		void RenderComponents();

		friend class GUIGameObject;
#endif

		void Serialize();
		void DeSerialize();
		void GetGraphPointers(int idOffset);

	protected:
		///The name of the GameObject
		String m_Name;

		///The GameObject's id
		GameObjectID m_GameObjectID;

		///A pointer to the parent
		WeakGameObjectPtr m_ParentNode;

		GameObjectID m_ParentID;

		///A GameObject vector containing weak pointer to the children
		GameObjectVector m_ChildrenVector;

		//A GameObject vector
		Vector<GameObjectID> m_ChildrenIDs;

		///The map for the component
		ComponentMap m_Components;

		///Position
		Vector3 m_Position;
		///Orientation
		Vector3 m_Orientation;
		///Scale
		Vector3 m_Scale;

		///The Accumulated Matrix
		Matrix4 m_ToWorld[2];

		///The Local Matrix
		Matrix4 m_ToLocal;

		//If this is true, something in the game object has changed and it needs to reconstruct the local matrix.
		bool m_DirtyLocalMatrix = true;

		///If the gameobject is to be destroyed.
		bool m_ToDestroy = false;
	};
};