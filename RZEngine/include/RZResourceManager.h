#pragma once

namespace rczEngine
{
	enum ResourceType
	{
		RES_UNKNOWN,
		RES_3DMODEL,
		RES_SKINNEDMODEL,
		RES_TEXTURE,
		RES_AUDIO,
		RES_MATERIAL,
		RES_ANIMATION,
		RES_CUBEMAP,
		RES_MISSING
	};

	class RZ_UTILITY_EXPORT ResVault
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
		ResourceHandle LoadResource(const char * filePath, const char* resName = NULL);
		void FreeResource(ResourceHandle handleToDelete);
		
		
		template <class type>
		WeakPtr<type> GetResource(ResourceHandle requestedHandle)
		{
			int handle;

			if (requestedHandle == -1)
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
		WeakPtr<type> FindResourceByName(char* name)
		{
			WeakPtr<type> res;

			for (auto i = m_ResourceMap.begin(); i != m_ResourceMap.end(); ++i)
			{
				if (i->second->m_Name == name)
				{
					res = std::static_pointer_cast<type, Resource>(i->second);
					return res;
				}
			}

			res = std::static_pointer_cast<type, Resource>(m_ResourceMap.at(0));
			return res;
		}

		ResourceHandle InsertResource(Resource* newRes);

		bool FileExists(const char *fileName)
		{
			std::ifstream infile(fileName);
			bool FileExists = infile.good();
			infile.close();
			return FileExists;
		}

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
		ResourceType QueryModel(const char* filePath, bool& out_hasBones, bool& out_hasTextures, bool& out_hasAnimations);
		ResourceType GetResourceType(const char* fileExtension);
	};

	ResourceHandle RZ_UTILITY_EXPORT LoadFile(char* instruction, const char* name, ResVault* res);
};