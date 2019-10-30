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
#ifdef PROFILING

#endif
		m_WhiteTex = LoadResource("RacrozEngineAssets/WhiteTex.tga", "WhiteTex");
		m_BlackTex = LoadResource("RacrozEngineAssets/BlackTex.tga", "BlackTex");
		m_GreyTex = LoadResource("RacrozEngineAssets/GreyTex.tga", "GreyTex");
		m_NormalTex = LoadResource("RacrozEngineAssets/NormalTex.png", "NormalTex");

		m_ModelCube = LoadResource("RacrozEngineAssets/CubePerlin.dae", "assetCube");
		m_ModelPlane = LoadResource("RacrozEngineAssets/Plane.fbx", "assetPlane");
		m_ModelSphere = LoadResource("RacrozEngineAssets/Showcase.fbx", "assetSphere");

		m_CubeMapDefault = LoadResource("CubeMaps/Enviroment.dds", "assetCubeMap");

		Logger::Pointer()->StartLog("Material");
	};

	void ResVault::Destroy()
	{
		m_ResourceMap.clear();
		//for (auto it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
		//{
		//	it->second->Release();
		//}
	};

	ResourceHandle ResVault::LoadResource(const String& filePath, const String& resName)
	{
		try
		{
			///Look for the Resource to see if it is already loaded and if it is, return that.
			for (auto it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
			{
				if (it->second->GetFilePath() == filePath)
				{
					Logger::Pointer()->LogMessageToFileLog("Material", "Found in map");
					return it->second->GetHandle();
				}
			}

			Logger::Pointer()->LogMessageToFileLog("Material", "Not found in map");

			///Parse the filepath to find the extension
			Vector<String> parsedFilePath = Parser::ParseToStrings<ANSICHAR>(filePath, ".", 0);
			String fileExtension = parsedFilePath[parsedFilePath.size() - 1];

			///If there's not a name parameter, set the resource name to its filePath minus the extension.
			String ResName;
			if (resName == "")
			{
				ResName = parsedFilePath[0];
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
				CastStaticPtr<CubeMap>(NewRes)->LoadCubeMapFromDDS(filePath, ResName);
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
					Logger::Pointer()->LogMessageToFileLog("Material", "File does not exist.");
					return m_WhiteTex;
				}
				Logger::Pointer()->LogMessageToFileLog("Material", String("Loading Texture: ") + String(filePath).c_str());

				NewRes = std::make_shared<Texture2D>();
				NewRes->Load(filePath, ResName);
				break;

			case RES_UNKNOWN:
				Logger::Pointer()->LogMessageToFileLog("Material", "Res Unknown");
				break;
			}

			Logger::Pointer()->LogMessageToFileLog("Material", "Handle: ", (float)NewRes->GetHandle());
			return NewRes->GetHandle();
		}
		catch (std::exception& exception)
		{
			std::cout<<exception.what();

		};
	};

	void ResVault::FreeResource(ResourceHandle handleToDelete)
	{
		m_ResourceMap.erase(handleToDelete);
	}

	void ResVault::LoadModel(StrPtr<GameObject> gameobject, const String& filePath, bool withHierarchy)
	{
		try
		{
			///Parse the filepath to find the extension
			Vector<String> parsedFilePath = Parser::ParseToStrings<ANSICHAR>(filePath, ".", 0);
			String fileExtension = parsedFilePath[parsedFilePath.size() - 1];


			UniquePtr<ModelLoader> loader;
			//if (fileExtension == "fbx")
			//{
			//	loader = std::make_unique<FbxLoader>();
			//}
			//else
			//{
				loader = std::make_unique<AssimpLoader>();
			//}

			bool bones = false;
			bool textures, animations;
			QueryModel(filePath, bones, textures, animations);

			if (bones)
			{
				loader->LoadSkinnedModel(gameobject, filePath);
			}
			else
			{
				if (withHierarchy)
					loader->LoadModelWithHierarchy(gameobject, filePath);
				else
					gameobject->GetComponent<ModelRenderer>(CMP_MODEL_RENDERER).lock()->SetModel(loader->LoadModel(filePath)->GetHandle(), this);
			}
		}
		catch (std::exception exc)
		{
			Logger::Pointer()->Log("Could not load model", eLogMsgType::Error);
		}
	}

	ResourceHandle ResVault::InsertResource(StrPtr<Resource> newRes)
	{
		ResourceHandle handle = newRes->GetHandle();
		m_ResourceMap[handle] = newRes;

		return handle;
	}

	ResourceType ResVault::QueryModel(const String& filePath, bool& out_hasBones, bool& out_hasTextures, bool& out_hasAnimations)
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

	ResourceType ResVault::GetResourceType(const String& fileExtension)
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

	ResourceHandle LoadFile(const String& instruction, const String& name, ResVault * res)
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
		ofn.lpstrTitle = instruction.c_str();
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
	};

	String GetFilePath(const String& instruction)
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
		ofn.lpstrTitle = instruction.c_str();
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn))
		{
			return filename;
		}
		else
		{
			std::cout << "You cancelled.\n";
		}

		return INVALID_RESOURCE;
	};

};