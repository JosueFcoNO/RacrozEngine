#include <RZEnginePCH.h>
#include <assimp/pbrmaterial.h>

namespace rczEngine
{
	StrPtr<Model> AssimpLoader::LoadModel(const String& filePath)
	{
		m_Res = ResVault::Pointer();

		m_FilePath = filePath;

		StrPtr<Model> model = std::make_shared<Model>();
		model->SetFilePath(filePath);
		model->GetPath().GetFileName();

		ResourceHandle handle = m_Res->InsertResource(model);

		///Clear the index and vertex buffer's lists
		model->m_IndexBuffer.ClearIndexList();
		model->m_VertexBuffer.ClearVertexList();
		model->m_VectorMeshes.clear();

		///Create an Importer and Read the file in fileName
		Assimp::Importer B;
		m_Scene = B.ReadFile(filePath,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_JoinIdenticalVertices |
			aiProcess_CalcTangentSpace |
			aiProcess_FlipUVs);

		LoadMaterials();
		model->m_MaterialMap = materialMap;

		///Set the VertexOffset, IndexOffset and IndexCount local variables to 0. 
		///These help creating the meshes and managing a single index and a single vertex buffer.
		uint32 VertexOffset = 0;
		uint32 IndexOffset = 0;
		uint32 IndexCount = 0;

		Mesh TempMesh;

		///Iterate the scene's meshes.
		for (uint32 n = 0; n < m_Scene->mNumMeshes; ++n)
		{
			///CurrentMesh = the mesh i'm currently iterating over.
			aiMesh* CurrentMesh = m_Scene->mMeshes[n];

			///Set the IndexCount to the CurrentMesh number of faces multiplied by 3. Each Triangle has 3 vertices and 3 indices
			IndexCount = CurrentMesh->mNumFaces * 3;

			///Create an object of my own Mesh
			TempMesh.m_MeshCore.m_IndexOffset = IndexOffset;
			TempMesh.m_MeshCore.m_IndexCount = IndexCount;
			TempMesh.m_MeshCore.m_VertexOffset = VertexOffset;
			TempMesh.m_Name = CurrentMesh->mName.C_Str();

			///Add the Current Mesh IndexCount to the Index Offset of this Mesh.
			IndexOffset += IndexCount;
			///Add the Current Mesh Number of vertices to the Number of vertices in the buffer
			VertexOffset += CurrentMesh->mNumVertices;

			///Get the material name and save it.
			aiString matName;
			m_Scene->mMaterials[CurrentMesh->mMaterialIndex]->Get(AI_MATKEY_NAME, matName);
			TempMesh.m_Material = matName.C_Str();

			///Insert the meshes in this model's meshes vector
			model->m_VectorMeshes.push_back(TempMesh);

			AddVertices(model->m_VertexBuffer, CurrentMesh);
			AddIndices(model->m_IndexBuffer, CurrentMesh);
		}

		Gfx::GfxCore* gfx = Gfx::GfxCore::Pointer();
		model->m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, false, gfx);
		model->m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, gfx);

		return model;
	}

	bool AssimpLoader::LoadModelWithHierarchy(StrGameObjectPtr gameObject, const String& filePath)
	{
		m_Res = ResVault::Pointer();

		m_FilePath = filePath;

		///Create an Importer and Read the file in fileName
		Assimp::Importer B;
		m_Scene = B.ReadFile(filePath,
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

		LoadMaterials();

		aiNode* lRootNode = m_Scene->mRootNode;

		auto node = SceneManager::Pointer()->GetActiveScene()->
			CreateActor(m_FilePath.GetFileName().c_str(), gameObject.get());

		if (lRootNode) {
			for (uint32 i = 0; i < lRootNode->mNumChildren; i++)
				AddNodeMesh(lRootNode->mChildren[i], m_FilePath.GetFileName().c_str(), true);
		}

		return true;
	}

	bool AssimpLoader::LoadSkinnedModel(StrGameObjectPtr gameObject, const String& filePath)
	{
		m_Res = ResVault::Pointer();
		m_Gfx = Gfx::GfxCore::Pointer();

		Logger::Pointer()->StartLog("ModelSkinned");

		StrPtr<SkinnedModel> model = std::make_shared<SkinnedModel>();
		model->SetFilePath(filePath);
		model->SetName(model->GetPath().GetFileName().c_str());
		ResourceHandle handle = m_Res->InsertResource(model);

		///Create an Importer and Read the file in fileName
		Assimp::Importer B;
		//B.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		m_Scene = B.ReadFile(filePath,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace |
			aiProcess_LimitBoneWeights |
			aiProcess_OptimizeMeshes
		);

		LoadMaterials();
		model->m_MaterialMap = materialMap;

		///Set the VertexOffset, IndexOffset and IndexCount local variables to 0. These help creating the meshes and managing a single index and a single vertex buffer.
		uint32 VertexOffset = 0;
		uint32 IndexOffset = 0;
		uint32 IndexCount = 0;

		for (uint32 n = 0; n < m_Scene->mNumMeshes; ++n)
		{
			///CurrentMesh = the mesh i'm currently iterating over.
			aiMesh* CurrentMesh = m_Scene->mMeshes[n];

			///Set the IndexCount to the CurrentMesh number of faces multiplied by 3. Each Triangle has 3 vertices and 3 indices
			IndexCount = CurrentMesh->mNumFaces * 3;

			///Create an object of my own Mesh
			SkinnedMesh TempMesh;
			TempMesh.m_MeshCore.m_IndexOffset = IndexOffset;
			TempMesh.m_MeshCore.m_IndexCount = IndexCount;
			TempMesh.m_MeshCore.m_VertexOffset = VertexOffset;
			TempMesh.m_Name = CurrentMesh->mName.C_Str();

			///Add the Current Mesh IndexCount to the Index Offset of this Mesh.
			IndexOffset += IndexCount;

			///Add the Current Mesh Number of vertices to the Number of vertices in the buffer
			VertexOffset += CurrentMesh->mNumVertices;

			///Get the material name and save it.
			aiString matName;
			m_Scene->mMaterials[CurrentMesh->mMaterialIndex]->Get(AI_MATKEY_NAME, matName);
			TempMesh.m_Material = matName.C_Str();

			model->m_MaterialMap[matName.C_Str()] = materialMap[matName.C_Str()];

			AddVertices(model->m_VertexBuffer, CurrentMesh);
			AddBones(model, CurrentMesh, TempMesh.m_MeshCore.m_VertexOffset);
			AddIndices(model->m_IndexBuffer, CurrentMesh);

			///Insert the meshes in the this model's meshes vector
			model->m_VectorMeshes.push_back(TempMesh);
		}

		BuildSkeleton(model);

		///Create a tempAnim pointer
		StrPtr<Animation> TempAnim = std::make_shared<Animation>();
		TempAnim->Load(filePath, "Anim");
		model->m_SetAnimation = m_Res->InsertResource(TempAnim);

		///Create the vertex and index buffers
		model->m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, false, m_Gfx);
		model->m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, m_Gfx);

		///Create the Skeleton's buffers
		model->m_MeshSkeleton.InitSkeletonBuffer(m_Gfx);

		gameObject->GetComponent<SkinnedModelRenderer>(CMP_SKINNED_MODEL_RENDERER).lock()->SetModel(model->GetHandle(), m_Res);

		aiNode* lRootNode = m_Scene->mRootNode;

		if (lRootNode)
		{
			for (uint32 i = 0; i < lRootNode->mNumChildren; i++)
				AddNodeMesh(lRootNode->mChildren[i], gameObject->GetName().c_str(), false, true);
		}

		for (auto it = BoneComponents.begin(); it != BoneComponents.end(); ++it)
		{
			Bone* boneToSet = model->m_MeshSkeleton.GetBone(it->first);
			if (boneToSet)
				boneToSet->InitBone(it->second);
		}

		return true;
	}

	bool AssimpLoader::AddNodeMesh(aiNode * pNode, const String& parent, bool addMeshes, bool addBones)
	{
		const String& nodeName = pNode->mName.C_Str();

		aiVector3D translation;
		aiVector3D rotation;
		aiVector3D scaling;

		pNode->mTransformation.Decompose(scaling, rotation, translation);

		Vector3 trans(translation.x, translation.y, translation.z);
		Vector3 rot(
			Math::RadiansToDegrees(-rotation.x),
			Math::RadiansToDegrees(-rotation.y),
			Math::RadiansToDegrees(-rotation.z));
		Vector3 scale(scaling.x, scaling.y, scaling.z);

		WeakGameObjPtr node;

		auto scene = SceneManager::Pointer()->GetActiveScene();

		if (parent == "")
		{
			node = SceneManager::Pointer()->GetActiveScene()->
				CreateActor(nodeName, NULL, trans, rot, scale);
		}
		else
		{
			auto ptrParent = scene->FindActor(parent);
			if (!ptrParent.expired())
			{
				node = scene->CreateActor(nodeName, ptrParent.lock().get(), trans, rot, scale);
			}
		}

		if (pNode->mNumMeshes && addMeshes)
		{
			ResourceHandle handle = LoadMesh(pNode, false);
			scene->CreateComponent(CMP_MODEL_RENDERER, node.lock());
			node.lock()->GetComponent<ModelRenderer>(CMP_MODEL_RENDERER).lock()->SetModel(handle, m_Res);
		}

		if (addBones)
		{
			BoneComponents[pNode->mName.C_Str()] = rczEngine::CastStaticPtr<BoneComponent>(scene->CreateComponent(CMP_BONE, node.lock()));
		}

		for (uint32 i = 0; i < pNode->mNumChildren; ++i)
		{
			AddNodeMesh(pNode->mChildren[i], node.lock()->GetName().c_str(), addMeshes, addBones);
		}

		return true;
	}

	void AssimpLoader::LoadMaterials()
	{
		for (uint32 i = 0; i < m_Scene->mNumTextures; ++i)
		{
			m_Scene->mTextures[i]->pcData;
		}

		///Adds the materials
		for (uint32 k = 0; k < m_Scene->mNumMaterials; ++k)
		{
			aiMaterial* aimatTemp = m_Scene->mMaterials[k];

			///Get the material's name
			aiString name;
			aimatTemp->Get(AI_MATKEY_NAME, name);

			StrPtr<Material> Temp = std::make_shared<Material>();
			Temp->SetName(name.C_Str());
			Temp->SetFilePath(String(m_Scene->mRootNode->mName.C_Str()) + "/" + name.C_Str());

			ResourceHandle tempHandle = m_Res->InsertResource(Temp);
			materialMap.insert(Pair<String, ResourceHandle>(Temp->GetName(), tempHandle));
			Temp->InitMaterial(MAT_PBR_MetRough, Gfx::GfxCore::Pointer());


			aiColor4D outV = { 0, 0, 0, 0 };
			aimatTemp->Get(AI_MATKEY_COLOR_SPECULAR, outV);
			Temp->m_core.g_Specular.Set(outV.r, outV.g, outV.b);
			Logger::Pointer()->Log("Spec R: " + std::to_string(outV.r));
			Logger::Pointer()->Log("Spec G: " + std::to_string(outV.g));
			Logger::Pointer()->Log("Spec B: " + std::to_string(outV.b));
			Logger::Pointer()->Log("Spec A: " + std::to_string(outV.a));

			//outV = { 0, 0, 0, 0 };
			//aimatTemp->Get(AI_MATKEY_COLOR_DIFFUSE, outV);
			//Temp->m_core.g_Albedo.Set(outV.r, outV.g, outV.b);
			//Logger::Pointer()->Log("Diff R: " + std::to_string(outV.r));
			//Logger::Pointer()->Log("Diff G: " + std::to_string(outV.g));
			//Logger::Pointer()->Log("Diff B: " + std::to_string(outV.b));
			//Logger::Pointer()->Log("Diff A: " + std::to_string(outV.a));
			//
			////outV = { 0, 0, 0, 0 };
			//aimatTemp->Get(AI_MATKEY_COLOR_AMBIENT, outV);
			//Temp->m_core.g_Albedo.Set(outV.r, outV.g, outV.b);
			//Logger::Pointer()->Log("Amb R: " + std::to_string(outV.r));
			//Logger::Pointer()->Log("Amb G: " + std::to_string(outV.g));
			//Logger::Pointer()->Log("Amb B: " + std::to_string(outV.b));
			//Logger::Pointer()->Log("Amb A: " + std::to_string(outV.a));
			//
			//outV = { 0, 0, 0, 0 };
			//aimatTemp->Get(AI_MATKEY_COLOR_REFLECTIVE, outV);
			//Temp->m_core.g_Albedo.Set(outV.r, outV.g, outV.b);
			//Logger::Pointer()->Log("Ref R: " + std::to_string(outV.r));
			//Logger::Pointer()->Log("ref G: " + std::to_string(outV.g));
			//Logger::Pointer()->Log("Ref B: " + std::to_string(outV.b));
			//Logger::Pointer()->Log("Ref A: " + std::to_string(outV.a));

			//outV = { 0, 0, 0, 0 };
			//aimatTemp->Get(AI_MATKEY_COLOR_EMISSIVE, outV);
			//Temp->m_core.g_Emmisive.Set(outV.r, outV.g, outV.b);
			//Logger::Pointer()->Log("Em R: " + std::to_string(outV.r));
			//Logger::Pointer()->Log("Em G: " + std::to_string(outV.g));
			//Logger::Pointer()->Log("Em B: " + std::to_string(outV.b));
			//Logger::Pointer()->Log("Em A: " + std::to_string(outV.a));
			//
			outV = { 0, 0, 0, 0 };
			aimatTemp->Get(AI_MATKEY_COLOR_TRANSPARENT, outV);
			//Temp->m_core.g_Opacity = outV.r;
			Logger::Pointer()->Log("Trans R: " + std::to_string(outV.r));
			Logger::Pointer()->Log("Trans G: " + std::to_string(outV.g));
			Logger::Pointer()->Log("Trans B: " + std::to_string(outV.b));
			Logger::Pointer()->Log("Trans A: " + std::to_string(outV.a));

			//ai_real out = 0.5;
			//aimatTemp->Get(AI_MATKEY_SHININESS, out);
			//Logger::Pointer()->Log("Gloss1: " + std::to_string(out));
			//
			//ai_real out2 = 0.5;
			//aimatTemp->Get(AI_MATKEY_SHININESS_STRENGTH, out2);
			//Logger::Pointer()->Log("Gloss2: " + std::to_string(out2));
			//
			//out2 = 0.5;
			//aimatTemp->Get(AI_MATKEY_REFLECTIVITY, out2);
			//Temp->m_core.g_RoughGloss = out2;
			//Logger::Pointer()->Log("Reflectivity: " + std::to_string(out2));
			//
			//out2 = 0.5;
			//aimatTemp->Get(AI_MATKEY_REFRACTI, out2);
			//Logger::Pointer()->Log("Refraction: " + std::to_string(out2));
			//
			//out2 = 0.5;
			//aimatTemp->Get(AI_MATKEY_BUMPSCALING, out2);
			//Logger::Pointer()->Log("BumpScaling: " + std::to_string(out2));
			//
			//out2 = 0.5;
			//aimatTemp->Get(AI_MATKEY_OPACITY, out2);
			//Logger::Pointer()->Log("Opacity: " + std::to_string(out2));

			aiString Path;
			aimatTemp->GetTexture(aiTextureType_DIFFUSE, 0, &Path);

			String path = Path.C_Str();
			String Dir = m_FilePath.GetFileDir();
			String RealPath;

			if (path != "")
			{
				RealPath = Dir + path.c_str();
				if (Path::FileExists(RealPath.c_str()))
					Temp->m_TextureAlbedo = m_Res->LoadResource(RealPath.c_str(), name.C_Str());
			}
			else
			{
				//Temp->m_core.OverrideAlbedo = 1.0f;
			}

			Path = "";
			aimatTemp->GetTexture(aiTextureType_NORMALS, 0, &Path);
			path = Path.C_Str();

			if (path != "")
			{
				RealPath = Dir + path.c_str();
				if (Path::FileExists(RealPath.c_str()))
					Temp->m_TextureNormal = m_Res->LoadResource(RealPath.c_str(), name.C_Str());
			}

			Path = "";
			aimatTemp->GetTexture(aiTextureType_SPECULAR, 0, &Path);
			path = Path.C_Str();

			if (path != "")
			{
				RealPath = Dir + path.c_str();
				if (Path::FileExists(RealPath.c_str()))
					Temp->m_TextureMetSpec = m_Res->LoadResource(RealPath.c_str(), name.C_Str());
			}
			else
			{
				Temp->m_core.OverrideMetallicSpecular = 1.0f;
				Temp->m_core.OverrideRoughGloss = 1.0f;
				Temp->m_core.g_RoughGloss = 0.5f;
			}


			Path = "";
			aimatTemp->GetTexture(aiTextureType_EMISSIVE, 0, &Path);
			path = Path.C_Str();
			if (path != "")
			{
				RealPath = Dir + path.c_str();
				if (Path::FileExists(RealPath.c_str()))
					Temp->m_TextureEm = m_Res->LoadResource(RealPath.c_str(), name.C_Str());
			}
			else
			{
				Temp->m_core.OverriveEmmisive = 1.0f;
			}


			Path = "";
			aimatTemp->GetTexture(aiTextureType_OPACITY, 0, &Path);
			path = Path.C_Str();
			if (path != "")
			{
				RealPath = Dir + path.c_str();
				if (Path::FileExists(RealPath.c_str()))
					Temp->m_TextureO = m_Res->LoadResource(RealPath.c_str(), name.C_Str());
			}
			else
			{
				Temp->m_core.g_Opacity = 0.5f;
			}

			for (uint32 i = 0; i < aimatTemp->GetTextureCount(aiTextureType_UNKNOWN); ++i)
			{
				Path = "";
				aimatTemp->GetTexture(aiTextureType::aiTextureType_UNKNOWN, i, &Path);
				path = Path.C_Str();
				if (path != "")
				{
					RealPath = Dir + path.c_str();
					if (Path::FileExists(RealPath.c_str()))
						Temp->m_TextureEm = m_Res->LoadResource(RealPath.c_str(), name.C_Str());
				}
			}

			for (uint32 i = 0; i < aimatTemp->GetTextureCount(aiTextureType_NONE); ++i)
			{
				Path = "";
				aimatTemp->GetTexture(aiTextureType::aiTextureType_NONE, i, &Path);
				path = Path.C_Str();
				if (path != "")
				{
					RealPath = Dir + path.c_str();
					if (Path::FileExists(RealPath.c_str()))
						Temp->m_TextureEm = m_Res->LoadResource(RealPath.c_str(), name.C_Str());
				}
			}
		}

		Logger::Pointer()->CloseLog("Material");
	}

	ResourceHandle AssimpLoader::LoadMesh(aiNode * pNode, bool bones)
	{
		m_Res = ResVault::Pointer();
		Gfx::GfxCore* gfx = Gfx::GfxCore::Pointer();

		StrPtr<Model> model = std::make_shared<Model>();

		model->SetFilePath(pNode->mName.C_Str());
		m_Res->InsertResource(model);

		///Clear the index and vertex buffer's lists
		model->m_IndexBuffer.ClearIndexList();
		model->m_VertexBuffer.ClearVertexList();
		model->m_VectorMeshes.clear();

		///Set the VertexOffset, IndexOffset and IndexCount local variables to 0. These help creating the meshes and managing a single index and a single vertex buffer.
		uint32 VertexOffset = 0;
		uint32 IndexOffset = 0;
		uint32 IndexCount = 0;

		///Iterate the scene's meshes.
		for (uint32 n = 0; n < pNode->mNumMeshes; ++n)
		{
			///CurrentMesh = the mesh i'm currently iterating over.
			aiMesh* CurrentMesh = m_Scene->mMeshes[pNode->mMeshes[n]];

			///Set the IndexCount to the CurrentMesh number of faces multiplied by 3. Each Triangle has 3 vertices and 3 indices
			IndexCount = CurrentMesh->mNumFaces * 3;

			///Create an object of my own Mesh
			Mesh TempMesh;
			TempMesh.m_MeshCore.m_IndexOffset = IndexOffset;
			TempMesh.m_MeshCore.m_IndexCount = IndexCount;
			TempMesh.m_MeshCore.m_VertexOffset = VertexOffset;
			TempMesh.m_Name = CurrentMesh->mName.C_Str();

			///Add the Current Mesh IndexCount to the Index Offset of this Mesh.
			IndexOffset += IndexCount;
			///Add the Current Mesh Number of vertices to the Number of vertices in the buffer
			VertexOffset += CurrentMesh->mNumVertices;

			///Get the material name and save it.
			aiString matName;
			m_Scene->mMaterials[CurrentMesh->mMaterialIndex]->Get(AI_MATKEY_NAME, matName);
			TempMesh.m_Material = matName.C_Str();

			model->m_MaterialMap[matName.C_Str()] = materialMap[matName.C_Str()];

			///Insert the meshes in the this model's meshes vector
			model->m_VectorMeshes.push_back(TempMesh);

			///Iterate through the vertices
			for (uint32 i = 0; i < CurrentMesh->mNumVertices; ++i)
			{
				///Create a temp Gfx::Vertex and assign it the CurrentMesh Gfx::Vertex[i] position, texcoords and normals
				Gfx::Vertex Temp;
				Temp.VertexPosition.m_x = CurrentMesh->mVertices[i].x;
				Temp.VertexPosition.m_y = CurrentMesh->mVertices[i].y;
				Temp.VertexPosition.m_z = CurrentMesh->mVertices[i].z;

				if (CurrentMesh->HasTextureCoords(0))
				{
					Temp.TextureCoordinates.m_x = CurrentMesh->mTextureCoords[0][i].x;
					Temp.TextureCoordinates.m_y = CurrentMesh->mTextureCoords[0][i].y;
				}

				Temp.VertexNormals.m_x = CurrentMesh->mNormals[i].x;
				Temp.VertexNormals.m_y = CurrentMesh->mNormals[i].y;
				Temp.VertexNormals.m_z = CurrentMesh->mNormals[i].z;

				if (CurrentMesh->HasTangentsAndBitangents())
				{
					Temp.Tangents.m_x = CurrentMesh->mTangents[i].x;
					Temp.Tangents.m_y = CurrentMesh->mTangents[i].y;
					Temp.Tangents.m_z = CurrentMesh->mTangents[i].z;

					Temp.BiNormals.m_x = CurrentMesh->mBitangents[i].x;
					Temp.BiNormals.m_y = CurrentMesh->mBitangents[i].y;
					Temp.BiNormals.m_z = CurrentMesh->mBitangents[i].z;
				}
				else
				{
					Temp.Tangents.m_x = 0;
					Temp.Tangents.m_y = 0;
					Temp.Tangents.m_z = 1;

					Temp.BiNormals.m_x = 0;
					Temp.BiNormals.m_y = 1;
					Temp.BiNormals.m_z = 0;
				}

				model->m_VertexBuffer.AddVertex(Temp);
			}

			for (uint32 o = 0; o < CurrentMesh->mNumFaces; ++o)
			{
				for (uint32 i = 0; i < CurrentMesh->mFaces[o].mNumIndices; ++i)
				{
					uint32 Index;
					Index = CurrentMesh->mFaces[o].mIndices[i];

					model->m_IndexBuffer.AddIndex(Index);
				}
			}
		}

		model->m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, false, gfx);
		model->m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, gfx);

		return model->GetHandle();
	}

	void AssimpLoader::BuildSkeleton(StrPtr<SkinnedModel> skin)
	{
		///create a depth and a minimal depth variable
		uint32 depth = 0, minDepth = 1000;
		///Create a RealRoot aiBone pointer
		aiBone* RealRoot;
		///Create a RealRoot aiNode pointer
		aiNode* RealRootNode = NULL;

		int i = 0;
		///iterate through my BoneList
		for (auto it = AssimpBone.begin(); it != AssimpBone.end(); ++it, ++i)
		{
			aiBone* CurrentBone = it->second;
			///Set the depth to 0
			depth = 0;
			///Create a currentNode aiNode pointer that points a its corresponding aiNode from the Scene graph
			aiNode* CurrentNode = m_Scene->mRootNode->FindNode(CurrentBone->mName);

			///while the currentNode has a valid parent
			while (CurrentNode->mParent)
			{
				///Increase by one the depth
				++depth;
				///Make the parent the new CurrentNode 
				CurrentNode = CurrentNode->mParent;
			}

			///If depth is less than minDepth
			if (depth < minDepth)
			{
				///Set minimal depth to depth
				minDepth = depth;
				///Set the RealRoot Bone to the BoneList[i]
				RealRoot = CurrentBone;
				///Set the RealRoot Node to the Node of this bone
				RealRootNode = m_Scene->mRootNode->FindNode(CurrentBone->mName);
			}
		}

		auto rootNodeName = String(RealRootNode->mName.C_Str());

		if (skin->m_MeshSkeleton.m_Bones.find(rootNodeName) == skin->m_MeshSkeleton.m_Bones.end())
		{
			///Create a tempRoot to insert the Skeleton's root bone
			Bone TempRoot;
			TempRoot.m_Name = rootNodeName;

			CopyMatrix(TempRoot.m_JointMatrix, RealRootNode->mTransformation);
			TempRoot.m_OffsetMatrix.Identity();

			TempRoot.SetParent(NULL);
			TempRoot.m_BoneIndex = 0;

			skin->m_MeshSkeleton.AddBone(TempRoot, TempRoot.m_Name);
			skin->m_MeshSkeleton.m_RootBone = &skin->m_MeshSkeleton.m_Bones[TempRoot.m_Name];
		}
		else
		{
			skin->m_MeshSkeleton.m_RootBone = &skin->m_MeshSkeleton.m_Bones.find(rootNodeName)->second;
		}

		///Create a Temp aiNode pointer to the scene node that has this bone's name.
		aiNode* RootNode = m_Scene->mRootNode->FindNode(rootNodeName.c_str());

		AddBoneToSkeleton(skin, RootNode);

		skin->m_MeshSkeleton.m_RootBone = skin->m_MeshSkeleton.GetBone(rootNodeName);

		skin->m_MeshSkeleton.m_BoneFinalMatrixVector.resize(skin->m_MeshSkeleton.m_Bones.size(), Matrix4::IDENTITY());

		Logger::Pointer()->CloseLog("ModelSkinned");

	}

	void AssimpLoader::AddVertices(Gfx::VertexBuffer<Gfx::Vertex>& vertexBuffer, aiMesh * mesh)
	{
		///Iterate through the vertices
		for (uint32 i = 0; i < mesh->mNumVertices; ++i)
		{
			///Create a temp Gfx::Vertex and assign it the CurrentMesh Gfx::Vertex[i] position, texcoords and normals
			Gfx::Vertex Temp;

			Temp.VertexPosition.m_x = mesh->mVertices[i].x;
			Temp.VertexPosition.m_y = mesh->mVertices[i].y;
			Temp.VertexPosition.m_z = mesh->mVertices[i].z;

			if (mesh->HasTextureCoords(0))
			{
				Temp.TextureCoordinates.m_x = mesh->mTextureCoords[0][i].x;
				Temp.TextureCoordinates.m_y = mesh->mTextureCoords[0][i].y;
			}

			Temp.VertexNormals.m_x = mesh->mNormals[i].x;
			Temp.VertexNormals.m_y = mesh->mNormals[i].y;
			Temp.VertexNormals.m_z = mesh->mNormals[i].z;

			if (mesh->HasTangentsAndBitangents())
			{
				Temp.Tangents.m_x = mesh->mTangents[i].x;
				Temp.Tangents.m_y = mesh->mTangents[i].y;
				Temp.Tangents.m_z = mesh->mTangents[i].z;

				Temp.BiNormals.m_x = mesh->mBitangents[i].x;
				Temp.BiNormals.m_y = mesh->mBitangents[i].y;
				Temp.BiNormals.m_z = mesh->mBitangents[i].z;
			}
			else
			{
				Temp.Tangents.m_x = 0;
				Temp.Tangents.m_y = 0;
				Temp.Tangents.m_z = 1;

				Temp.BiNormals.m_x = 0;
				Temp.BiNormals.m_y = 1;
				Temp.BiNormals.m_z = 0;
			}

			vertexBuffer.AddVertex(Temp);
		}
	}

	void AssimpLoader::AddVertices(Gfx::VertexBuffer<Gfx::SkinnedVertex>& vertexBuffer, aiMesh * mesh)
	{
		///Iterate through the vertices
		for (uint32 i = 0; i < mesh->mNumVertices; ++i)
		{
			///Create a temp Gfx::Vertex and assign it the CurrentMesh Gfx::Vertex[i] position, texcoords and normals
			Gfx::SkinnedVertex Temp;

			Temp.VertexPosition.m_x = mesh->mVertices[i].x;
			Temp.VertexPosition.m_y = mesh->mVertices[i].y;
			Temp.VertexPosition.m_z = mesh->mVertices[i].z;

			if (mesh->HasTextureCoords(0))
			{
				Temp.TextureCoordinates.m_x = mesh->mTextureCoords[0][i].x;
				Temp.TextureCoordinates.m_y = mesh->mTextureCoords[0][i].y;
			}

			Temp.VertexNormals.m_x = mesh->mNormals[i].x;
			Temp.VertexNormals.m_y = mesh->mNormals[i].y;
			Temp.VertexNormals.m_z = mesh->mNormals[i].z;

			if (mesh->HasTangentsAndBitangents())
			{
				Temp.Tangents.m_x = mesh->mTangents[i].x;
				Temp.Tangents.m_y = mesh->mTangents[i].y;
				Temp.Tangents.m_z = mesh->mTangents[i].z;

				Temp.BiNormals.m_x = mesh->mBitangents[i].x;
				Temp.BiNormals.m_y = mesh->mBitangents[i].y;
				Temp.BiNormals.m_z = mesh->mBitangents[i].z;
			}
			else
			{
				Temp.Tangents.m_x = 0;
				Temp.Tangents.m_y = 0;
				Temp.Tangents.m_z = 1;

				Temp.BiNormals.m_x = 0;
				Temp.BiNormals.m_y = 1;
				Temp.BiNormals.m_z = 0;
			}

			vertexBuffer.AddVertex(Temp);
		}
	}

	void AssimpLoader::AddIndices(Gfx::IndexBuffer& indexBuffer, aiMesh * mesh)
	{
		///Iterate through the faces
		for (uint32 o = 0; o < mesh->mNumFaces; ++o)
		{
			///Get every index of every face
			for (uint32 i = 0; i < mesh->mFaces[o].mNumIndices; ++i)
			{
				uint32 Index;
				Index = mesh->mFaces[o].mIndices[i];

				indexBuffer.AddIndex(Index);
			}
		}

	}

	void AssimpLoader::AddBones(StrPtr<SkinnedModel> model, aiMesh * mesh, uint32 vertexOffset)
	{
		///Check to see if it has bones
		if (mesh->HasBones())
		{
			///Iterate through the bones
			for (uint32 i = 0; i < mesh->mNumBones; ++i)
			{
				///Create pointer CurrentBone to mBones[i]
				aiBone* currentBone = mesh->mBones[i];
				auto currentBoneName = String(currentBone->mName.C_Str());

				///Create a newBone
				Bone newBone;

				Logger::Pointer()->LogMessageToFileLog("ModelSkinned", String("Adding Bone: ") + currentBone->mName.C_Str());

				//Bone does not exist yet.
				if (AssimpBone.find(currentBoneName) == AssimpBone.end())
				{
					///Give it the current bone's name
					newBone.m_Name = currentBoneName;

					///Set the TempBone's offset matrix to the BoneList[i]'s offset matrix.
					CopyMatrix(newBone.m_OffsetMatrix, currentBone->mOffsetMatrix);

					newBone.m_RealBone = true;

					newBone.m_BoneIndex = i + 1;

					BoneNames.push_back(currentBoneName);
					///Save all the bones in a bone list
					AssimpBone[currentBoneName] = currentBone;

					model->m_MeshSkeleton.m_Bones[currentBoneName] = newBone;

					Logger::Pointer()->LogMessageToFileLog("ModelSkinned", String("Bone not in vector."));
					Logger::Pointer()->LogMessageToFileLog("ModelSkinned", ("Index: "), i + 1);

				}

				Logger::Pointer()->LogMessageToFileLog("ModelSkinned", ("Using Index: "), newBone.m_BoneIndex);

				///Create a temp Gfx::SkinnedVertex pointer
				Gfx::SkinnedVertex* SkndVertex;
				Logger::Pointer()->LogMessageToFileLog("ModelSkinned", ("Bone Has Weights: "), currentBone->mNumWeights);

				///Iterate through the weights in my bone
				for (uint32 o = 0; o < currentBone->mNumWeights; ++o)
				{
					///Get the vertex in this aiWeight.VertexId
					SkndVertex = &model->m_VertexBuffer.GetVertex(currentBone->mWeights[o].mVertexId + vertexOffset);

					///iterate through the 4 Gfx::Vertex's indices and weights
					for (int32 k = 0; k < 4; ++k)
					{
						///if the Bone index is not set
						if (SkndVertex->BoneIndex[k] < 0)
						{
							///Set the bone index and the corresponding weight in the skinnedVertex
							SkndVertex->BonesWeights[k] = currentBone->mWeights[o].mWeight;
							SkndVertex->BoneIndex[k] = newBone.m_BoneIndex;

							break;
						}
					}
				}

			}
		}
	}

	void AssimpLoader::AddBoneToSkeleton(StrPtr<SkinnedModel> model, aiNode * pNode)
	{
		auto BoneName = String(pNode->mName.C_Str());

		///get the bone from my own skeleton's map that contains the childrenStrings[o]
		Bone* currentBone = nullptr;
		if (model->m_MeshSkeleton.m_Bones.find(BoneName) != model->m_MeshSkeleton.m_Bones.end())
			currentBone = &model->m_MeshSkeleton.m_Bones.at(BoneName);

		if (currentBone == nullptr)
		{
			Bone NoBone;
			NoBone.m_Name = BoneName;

			NoBone.m_OffsetMatrix.Identity();

			NoBone.m_BoneIndex = model->m_MeshSkeleton.m_Bones.size() + 1;
			NoBone.m_RealBone = false;

			model->m_MeshSkeleton.AddBone(NoBone, BoneName);
			BoneNames.push_back(NoBone.m_Name.c_str());

			currentBone = model->m_MeshSkeleton.GetBone(BoneName);
		}

		CopyMatrix(currentBone->m_JointMatrix, pNode->mTransformation);

		auto parent = model->m_MeshSkeleton.GetBone(pNode->mParent->mName.C_Str());

		if (pNode->mParent && parent)
			parent->AddBoneChildren(currentBone);

		currentBone->SetParent(parent);

		///Iterate through the string of the children's name
		for (uint32 o = 0; o < pNode->mNumChildren; ++o)
		{
			AddBoneToSkeleton(model, pNode->mChildren[o]);
		}
	}

	void AssimpLoader::CopyMatrix(Matrix4 & dest, aiMatrix4x4 & src)
	{
		//memcpy(&dest.m_elements, &src, sizeof(Matrix4));
		aiVector3D translation;
		aiVector3D rotation;
		aiVector3D scaling;

		src.Decompose(scaling, rotation, translation);

		Vector3 newPos(translation.x, translation.y, translation.z);
		Vector3 newRot(
			Math::RadiansToDegrees(-rotation.x),
			Math::RadiansToDegrees(-rotation.y),
			Math::RadiansToDegrees(-rotation.z));
		Vector3 newScale(scaling.x, scaling.y, scaling.z);

		switch (GUIEditor::Pointer()->SkinMode)
		{
		default:
		case 0:
			dest = Matrix4::Rotate3D(newRot.m_x, newRot.m_y, newRot.m_z)*Matrix4::Scale3D(newScale.m_x, newScale.m_y, newScale.m_z)*Matrix4::Translate3D(newPos.m_x, newPos.m_y, newPos.m_z);
			break;
		case 1:
			dest = Matrix4::Rotate3D(newRot.m_x, newRot.m_y, newRot.m_z)*Matrix4::Translate3D(newPos.m_x, newPos.m_y, newPos.m_z)*Matrix4::Scale3D(newScale.m_x, newScale.m_y, newScale.m_z);
			break;
		case 2:
			dest = Matrix4::Translate3D(newPos.m_x, newPos.m_y, newPos.m_z) * Matrix4::Rotate3D(newRot.m_x, newRot.m_y, newRot.m_z) * Matrix4::Scale3D(newScale.m_x, newScale.m_y, newScale.m_z);
			break;
		case 3:
			dest = Matrix4::Scale3D(newScale.m_x, newScale.m_y, newScale.m_z) * Matrix4::Rotate3D(newRot.m_x, newRot.m_y, newRot.m_z) * Matrix4::Translate3D(newPos.m_x, newPos.m_y, newPos.m_z);
			break;
		}
	}
}