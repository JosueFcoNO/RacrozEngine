#pragma once

namespace rczEngine
{
	class RZ_EXP ResVault
	{
	private:
		static ResVault*& _Instance();

	public:
		static void Start();
		static ResVault* Pointer();
		static void ShutDown();

		~ResVault() { Destroy(); };

		///Loads the standard and default assets.
		void InitResourceManager();
		void Destroy();
		ResourceHandle IsResourceLoaded(const String & filePath);
		ResourceHandle LoadResource(const String& filePath, const String& resName = NULL);
		void FreeResource(ResourceHandle handleToDelete);
		void LoadModel(StrPtr<GameObject> gameobject, const String& filePath, bool withHierarchy = false);


		template <class type>
		WeakPtr<type> GetResource(ResourceHandle requestedHandle)
		{
			size_t handle;

			if (requestedHandle == NULL)
			{
				handle = m_ModelCube;
			}
			else
			{
				handle = requestedHandle;
			}

			WeakPtr<type> res;
			res = std::static_pointer_cast<type, Resource>(m_ResourceMap.at(handle));
			return res;
		}

		template <class type>
		WeakPtr<type> FindResourceByName(const String& name)
		{
			WeakPtr<type> res;

			for (auto i = m_ResourceMap.begin(); i != m_ResourceMap.end(); ++i)
			{
				if (String(i->second->GetName()) == name)
				{
					res = std::static_pointer_cast<type, Resource>(i->second);
					return res;
				}
			}

			res = std::static_pointer_cast<type, Resource>(m_ResourceMap.at(0));
			return res;
		}

		ResourceHandle InsertResource(StrPtr<Resource> newRes);

		ResourceHandle m_UIMat;

		ResourceHandle m_WhiteTex;
		ResourceHandle m_BlackTex;
		ResourceHandle m_GreyTex;
		ResourceHandle m_NormalTex;

		ResourceHandle m_CubeMapDefault;

		ResourceHandle m_ModelPlane;
		ResourceHandle m_ModelCube;
		ResourceHandle m_ModelSphere;
		ResourceHandle m_ModelDefault;

		UoMap<ResourceHandle, StrPtr<Resource>> m_ResourceMap;

	private:

		uint32 m_iResourceIndex = 0;
		ResourceType QueryModel(const String& filePath, bool& out_hasBones, bool& out_hasTextures, bool& out_hasAnimations);
		ResourceType GetResourceType(const String& fileExtension);

		static const String LOG_ResourceVault;
	};

	String RZ_EXP GetFilePath(const String& instruction);
	ResourceHandle RZ_EXP LoadFile(const String& instruction, const String& name, ResVault* res);

};