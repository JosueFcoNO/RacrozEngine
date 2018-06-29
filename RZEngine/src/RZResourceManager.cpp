#include <RZEnginePCH.h>

namespace rczEngine
{
	//A constant array of all the Model formats the resource manager can load.
	const String ModelFormats[] =
	{
		{ "fbx" },
		{ "3ds" },
		{ "obj" },
		{ "dae" },
		{ "gltf" },
		{ "glb" },
		{ "blend" },
		{ "ase" },
		{ "ifc" },
		{ "xgl" },
		{ "zgl" },
		{ "ply" },
		{ "lwo" },
		{ "lws" },
		{ "lxo" },
		{ "stl" },
		{ "x" },
		{ "ac" },
		{ "ms3d" },
		{ "xml" },
		{ "irrmesh" },
		{ "mdl" },
		{ "md2" },
		{ "md3" },
		{ "pk3" },
		{ "md5" }
	};

	//A constant array of all the texture formats the resource manager can load.
	const String TextureFormats[] =
	{
		{ "png" },
		{ "bmp" },
		{ "jpg" },
		{ "tga" },
		{ "psd" },
		{ "gif" },
		{ "hdr" },
		{ "pic" },
		{ "jpeg"}
	};

	ResVault*& ResVault::_Instance()
	{
		static ResVault* instance = nullptr;
		return instance;
	}

	void ResVault::Start()
	{
		(_Instance()) = new ResVault;
	}

	ResVault* ResVault::Pointer()
	{
		return _Instance();
	}

	void ResVault::ShutDown()
	{
		delete _Instance();
	}

	void ResVault::InitResourceManager()
	{
		m_WhiteTex = LoadResource("RacrozEngineAssets/WhiteTex.tga", "WhiteTex");
		m_BlackTex = LoadResource("RacrozEngineAssets/BlackTex.tga", "BlackTex");
		m_GreyTex = LoadResource("RacrozEngineAssets/GreyTex.tga", "GreyTex");
		m_NormalTex = LoadResource("RacrozEngineAssets/NormalTex.png", "NormalTex");

		m_ModelCube = LoadResource("RacrozEngineAssets/CubePerlin.dae", "assetCube");
		m_ModelPlane = LoadResource("RacrozEngineAssets/Plane.fbx", "assetPlane");
		m_ModelSphere = LoadResource("RacrozEngineAssets/Showcase.fbx", "assetSphere");

		m_CubeMapDefault = LoadResource("CubeMaps/Enviroment.dds", "assetCubeMap");

	};

	void ResVault::Destroy()
	{
		for (auto it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
		{
			it->second->Release();
		}
	};

	ResourceHandle ResVault::LoadResource(const char * filePath, const char* resName)
	{
		///Look for the Resource to see if it is already loaded and if it is, return that.
		for (auto it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
		{
			if (it->second->GetFilePath() == filePath)
			{
				return it->second->GetHandle();
			}
		}

		///Parse the filepath to find the extension
		Vector<String> parsedFilePath = Parser::ParseToStrings<ANSICHAR>(filePath, ".", 0);
		String fileExtension = parsedFilePath[parsedFilePath.size() - 1];

		///If there's not a name parameter, set the resource name to its filePath minus the extension.
		const char* ResName;
		if (resName == NULL)
		{
			ResName = parsedFilePath[0].c_str();
		}
		else
		{
			ResName = resName;
		}

		StrPtr<Resource> NewRes;

		bool textures = false, animations = false, bones = false;

		bool fileExists = Path::FileExists(filePath);

		///Make the extension lower case.
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

		///Get the resource type from the extension and put it in a switch
		switch (GetResourceType(fileExtension.c_str()))
		{
		case RES_CUBEMAP:

			if (!fileExists)
			{
				return m_CubeMapDefault;
			}

			NewRes = std::make_shared<CubeMap>();
			CastStaticPtr<CubeMap>(NewRes)->LoadCubeMapFromDDS(filePath, (char*)ResName);
			break;

		case RES_3DMODEL:

			if (!fileExists)
			{
				return m_ModelCube;
			}

			QueryModel(filePath, bones, textures, animations);

			if (bones)
			{
				NewRes = std::make_shared<SkinnedModel>();
				NewRes->Load(filePath, ResName);
			}
			else
			{
				NewRes = std::make_shared<Model>();
				NewRes->Load(filePath, ResName);
			}
			break;

		case RES_TEXTURE:
			if (!fileExists)
			{
				return m_WhiteTex;
			}

			NewRes = std::make_shared<Texture2D>();
			NewRes->Load(filePath, (char*)ResName);
			break;

		case RES_UNKNOWN:
			break;
		}

		return NewRes->GetHandle();
	};

	void ResVault::FreeResource(ResourceHandle handleToDelete)
	{
		m_ResourceMap.erase(handleToDelete);
	}

	ResourceHandle ResVault::InsertResource(StrPtr<Resource> newRes)
	{
		ResourceHandle handle = newRes->GetHandle();
		m_ResourceMap[handle] = newRes;

		return handle;
	}

	ResourceType ResVault::QueryModel(const char* filePath, bool& out_hasBones, bool& out_hasTextures, bool& out_hasAnimations)
	{
		Assimp::Importer Imp;
		const aiScene* boneChecker = Imp.ReadFile(filePath, aiProcess_Triangulate);

		if (boneChecker == nullptr)
		{
			return RES_MISSING;
		}

		for (uint32 n = 0; n < boneChecker->mNumMeshes; ++n)
		{
			if (boneChecker->mMeshes[n]->HasBones())
			{
				out_hasBones = true;
			}
		}

		out_hasAnimations = boneChecker->HasAnimations();
		out_hasTextures = boneChecker->HasTextures();

		return RES_3DMODEL;
	}

	ResourceType ResVault::GetResourceType(const char * fileExtension)
	{
		if (fileExtension == (const char*)"dds")
		{
			return ResourceType::RES_CUBEMAP;
		}

		///Check to see if it is a model.
		for (int32 i = 0; i < ARRAYSIZE(ModelFormats); ++i)
		{
			///Check the extension.
			if (fileExtension == ModelFormats[i])
			{
				return ResourceType::RES_3DMODEL;
			}
		}

		///Check to see if it is a texture.
		for (int32 i = 0; i < ARRAYSIZE(TextureFormats); ++i)
		{
			///Check the extension.
			if (fileExtension == TextureFormats[i])
			{
				return ResourceType::RES_TEXTURE;
			}
		}

		return RES_CUBEMAP;
	}

	ResourceHandle LoadFile(char * instruction, const char * name, ResVault * res)
	{
		ANSICHAR filename[MAX_PATH];

		OPENFILENAME ofn;
		ZeroMemory(filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
		ofn.lpstrFilter = "Any File\0*.*\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = instruction;
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn))
		{
			return res->LoadResource(filename, name);
		}
		else
		{
			std::cout << "You cancelled.\n";
		}

		return INVALID_RESOURCE;
	}


};