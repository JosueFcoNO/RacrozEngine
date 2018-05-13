#include <RZEnginePCH.h>

namespace rczEngine
{
	StrPtr<Model> AssimpLoader::LoadModel(const char * filePath)
	{
		m_Res = ResVault::Pointer();
		Gfx::GfxCore* gfx = Gfx::GfxCore::Pointer();

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
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

		LoadMaterials(model->m_MaterialMap);

		///Set the VertexOffset, IndexOffset and IndexCount local variables to 0. These help creating the meshes and managing a single index and a single vertex buffer.
		uint32 VertexOffset = 0;
		uint32 IndexOffset = 0;
		uint32 IndexCount = 0;

		///Iterate the scene's meshes.
		for (uint32 n = 0; n < m_Scene->mNumMeshes; ++n)
		{
			///CurrentMesh = the mesh i'm currently iterating over.
			aiMesh* CurrentMesh = m_Scene->mMeshes[n];

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

		model->m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, gfx);
		model->m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, true, gfx);
		return model;
	}

	bool AssimpLoader::LoadModelWithHierarchy(StrGameObjectPtr gameObject, const char * filePath)
	{
		m_Res = ResVault::Pointer();
		Gfx::GfxCore* gfx = Gfx::GfxCore::Pointer();

		StrPtr<Model> model = std::make_shared<Model>();

		model->SetFilePath(filePath);

		ResourceHandle handle = m_Res->InsertResource(model);

		///Clear the index and vertex buffer's lists
		model->m_IndexBuffer.ClearIndexList();
		model->m_VertexBuffer.ClearVertexList();
		model->m_VectorMeshes.clear();

		///Create an Importer and Read the file in fileName
		Assimp::Importer B;
		m_Scene = B.ReadFile(filePath,
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

		aiNode* lRootNode = m_Scene->mRootNode;

		auto node = SceneManager::Pointer()->GetActiveScene()->
			CreateActor(model->GetPath().GetFileName().c_str(), NULL);

		if (lRootNode) {
			for (int i = 0; i < lRootNode->mNumChildren; i++)
				AddNodeMesh(lRootNode->mChildren[i], lRootNode->mName.C_Str());
		}

		return true;
	}

	StrPtr<SkinnedModel> AssimpLoader::LoadSkinnedModel(const char * filePath)
	{
		ResVault* rsc = ResVault::Pointer();
		Gfx::GfxCore* gfx = Gfx::GfxCore::Pointer();

		Vector<String> BoneNames;

		StrPtr<SkinnedModel> model = std::make_shared<SkinnedModel>();

		model->SetFilePath(filePath);
		model->SetName(model->GetPath().GetFileName().c_str());

		ResourceHandle handle = m_Res->InsertResource(model);

		///Clear the index and vertex buffer's lists
		model->m_IndexBuffer.ClearIndexList();
		model->m_VertexBuffer.ClearVertexList();
		model->m_VectorMeshes.clear();

		///Create an Importer and Read the file in fileName
		Assimp::Importer B;
		const aiScene* Scene = B.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights);

		LoadMaterials(model->m_MaterialMap);

		///Set the VertexOffset, IndexOffset and IndexCount local variables to 0. These help creating the meshes and managing a single index and a single vertex buffer.
		uint32 VertexOffset = 0;
		uint32 IndexOffset = 0;
		uint32 IndexCount = 0;

		Vector<aiBone*> BoneList;

		for (uint32 n = 0; n < Scene->mNumMeshes; ++n)
		{

			///CurrentMesh = the mesh i'm currently iterating over.
			aiMesh* CurrentMesh = Scene->mMeshes[n];

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
			Scene->mMaterials[CurrentMesh->mMaterialIndex]->Get(AI_MATKEY_NAME, matName);
			TempMesh.m_Material = matName.C_Str();

			///Insert the meshes in the this model's meshes vector
			//m_VectorMeshes.push_back(TempMesh);

			///Iterate through the vertices
			for (uint32 i = 0; i < CurrentMesh->mNumVertices; ++i)
			{
				///Create a temp Gfx::Vertex and assign it the CurrentMesh Gfx::Vertex[i] position, texcoords and normals
				Gfx::SkinnedVertex Temp;
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

			///Check to see if it has bones
			if (CurrentMesh->HasBones())
			{
				///Iterate through the bones
				for (uint32 i = 0; i < CurrentMesh->mNumBones; ++i)
				{
					///Create pointer CurrentBone to mBones[i]
					aiBone* CurrentBone = CurrentMesh->mBones[i];
					bool boneAlreadyInVector = false;

					for (int o = 0; o < BoneList.size(); ++o)
					{
						if (BoneList[o] == CurrentBone)
						{
							boneAlreadyInVector = true;
							break;
						}
					}

					if (boneAlreadyInVector == false)
					{
						///Save all the bones in a bone list
						BoneList.push_back(CurrentBone);
						BoneNames.push_back(CurrentBone->mName.C_Str());
					}
					else
					{
						continue;
					}

					///Create a temp Gfx::SkinnedVertex pointer
					Gfx::SkinnedVertex* SkndVertex;
					///Iterate through the weights in my bone
					for (uint32 o = 0; o < CurrentBone->mNumWeights; ++o)
					{
						///Get the vertex in this aiWeight.VertexId
						SkndVertex = &model->m_VertexBuffer.GetVertex(CurrentBone->mWeights[o].mVertexId + TempMesh.m_MeshCore.m_VertexOffset);

						///iterate through the 4 Gfx::Vertex's indices and weights
						for (int32 k = 0; k < 4; ++k)
						{
							///if the Bone index is not set
							if (SkndVertex->BoneIndex[k] < 0)
							{
								///Set the bone index and the corresponding weight in the skinnedVertex
								SkndVertex->BonesWeights[k] = CurrentBone->mWeights[o].mWeight;
								SkndVertex->BoneIndex[k] = int32(BoneList.size());

								break;
							}
						}
					}

				}
			}

			///Iterate through the faces
			for (uint32 o = 0; o < CurrentMesh->mNumFaces; ++o)
			{
				///Get every index of every face
				for (uint32 i = 0; i < CurrentMesh->mFaces[o].mNumIndices; ++i)
				{
					uint32 Index;
					Index = CurrentMesh->mFaces[o].mIndices[i];

					model->m_IndexBuffer.AddIndex(Index);
				}
			}

			///Insert the meshes in the this model's meshes vector
			model->m_VectorMeshes.push_back(TempMesh);

		}

		///create a depth and a minimal depth variable
		uint32 depth = 0, minDepth = 1000;
		///Create a RealRoot aiBone pointer
		aiBone* RealRoot;
		///Create a RealRoot aiNode pointer
		aiNode* RealRootNode = NULL;

		///iterate through my BoneList
		for (int32 i = 0; i < BoneList.size(); ++i)
		{
			aiBone* CurrentBone = BoneList[i];
			///Set the depth to 0
			depth = 0;
			///Create a currentNode aiNode pointer that points a its corresponding aiNode from the Scene graph
			aiNode* CurrentNode = Scene->mRootNode->FindNode(CurrentBone->mName);

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
				RealRootNode = Scene->mRootNode->FindNode(CurrentBone->mName);
			}

			///Create a TempBone
			Bone TempBone;
			///Give it the current bone's name
			TempBone.m_Name = String(CurrentBone->mName.data);
			TempBone.m_BoneIndex = i + 1;

			///Set the TempBone's offset matrix to the BoneList[i]'s offset matrix.
			memcpy(&TempBone.m_OffsetMatrix, &CurrentBone->mOffsetMatrix, sizeof(Matrix4::m_elements));

			///Add the Temp bone to the m_Skeleton's Bone Map
			model->m_MeshSkeleton.AddBone(TempBone, CurrentBone->mName.C_Str());
		}

		///Set the real root Node to its parent
		if (RealRootNode->mParent)
		{
			//RealRootNode = RealRootNode->mParent;
			//
			//do
			//{
			//	auto ParentName = Parser::ParseToStrings<ANSICHAR>(RealRootNode->mName.data, "$", 0);
			//	if (ParentName.size() > 2)
			//	{
			//		if (ParentName[1] == "AssimpFbx")
			//		RealRootNode = RealRootNode->mParent;
			//	}
			//	else
			//	{
			//		break;
			//	}
			//} while (1);
		}

		///If its named like its parent, change the parent's name
		if (RealRootNode->mParent)
		{
			if (RealRootNode->mParent->mName == RealRootNode->mName)
			{
				RealRootNode->mParent->mName.Append("1");
			}
		}

		///Create a tempRoot to insert the Skeleton's root bone
		Bone TempRoot;
		TempRoot.m_Name = RealRootNode->mName.C_Str();
		TempRoot.m_OffsetMatrix.Identity();
		TempRoot.m_JointMatrix.Identity();
		memcpy(&TempRoot.m_TransformMatrix, &RealRootNode->mTransformation, sizeof(Matrix4::m_elements));
		TempRoot.m_AccumulatedTransform.Identity();

		TempRoot.SetParent(NULL);
		TempRoot.m_BoneIndex = 0;
		model->m_MeshSkeleton.AddBone(TempRoot, TempRoot.m_Name);
		model->m_MeshSkeleton.m_RootBone = &model->m_MeshSkeleton.m_Bones[TempRoot.m_Name];
		BoneNames.push_back(TempRoot.m_Name);

		///Create the hierarchy in my own skeleton
		for (int32 i = 0; i < BoneNames.size(); ++i)
		{
			///Create a vector of strings to save all the children's names
			Vector<String> childrenStrings;
			///Create a Temp aiNode pointer to the scene node that has this bone's name.
			aiNode* Temp = Scene->mRootNode->FindNode(BoneNames[i].c_str());
			///Create a temp bone
			Bone* BoneTemp = &model->m_MeshSkeleton.m_Bones[BoneNames[i]];

			////Esto no tiene sentido, pero lo tenía cuando funcionaba bien.
			//memcpy(&BoneTemp->m_JointMatrix, &Temp->mTransformation, sizeof(Matrix4));

			Matrix4 PreRotation(INIT_UNIT);
			Matrix4 Rotation(INIT_UNIT);
			Matrix4 Translation(INIT_UNIT);
			Matrix4 Scale(INIT_UNIT);

			///Check to see if this bone has prerotation and translation
			if (Temp->mParent)
			{
				auto CurrentParent = Temp->mParent;
				auto ParentName = Parser::ParseToStrings<ANSICHAR>(Temp->mParent->mName.data, "$", 0);

				if (ParentName.size() > 2)
				{
					while (ParentName[1] == "AssimpFbx")
					{

						if (ParentName[ParentName.size() - 1] == "_PreRotation")
						{

							memcpy(&PreRotation, &CurrentParent->mTransformation, sizeof(Matrix4));

						}
						else if (ParentName[ParentName.size() - 1] == "_Translation")
						{
							memcpy(&Translation, &CurrentParent->mTransformation, sizeof(Matrix4));

						}
						else if (ParentName[ParentName.size() - 1] == "_Rotation")
						{
							memcpy(&Rotation, &CurrentParent->mTransformation, sizeof(Matrix4));
						}
						else
						{
							memcpy(&Scale, &CurrentParent->mTransformation, sizeof(Matrix4));
						}

						if (CurrentParent->mParent)
						{
							CurrentParent = CurrentParent->mParent;
						}

						ParentName = Parser::ParseToStrings<ANSICHAR>(CurrentParent->mName.data, "$", 0);
						if (ParentName.size() < 3)
						{
							Matrix4 Tempi(INIT_NONE);
							memcpy(&BoneTemp->m_TransformMatrix, &Temp->mTransformation, sizeof(Matrix4));
							BoneTemp->m_TransformMatrix = (Scale*Rotation*Translation*PreRotation);
							BoneTemp->m_JointIsTransform = false;
							break;
						}
					}
				}
				else
				{
					BoneTemp->m_JointIsTransform = true;
					memcpy(&BoneTemp->m_TransformMatrix, &Temp->mTransformation, sizeof(Matrix4));
				}

			}

			///Iterate through the string of the children's name
			for (int32 o = 0; o < Temp->mNumChildren; ++o)
			{
				aiNode* TempChildrenNode = Temp->mChildren[o];
				auto Childrens = Parser::ParseToStrings<ANSICHAR>(TempChildrenNode->mName.data, "$", 0);

				if (Childrens.size() > 1)
				{
					while (Childrens[1] == "AssimpFbx")
					{
						TempChildrenNode = TempChildrenNode->mChildren[0];
						Childrens = Parser::ParseToStrings<ANSICHAR>(TempChildrenNode->mName.data, "$", 0);

						if (Childrens.size() < 2)
						{
							break;
						}
					}
				}

				///Put them in the vector
				childrenStrings.push_back(TempChildrenNode->mName.C_Str());


			}

			///Iterate throuhg the childrenStrings
			for (int o = 0; o < childrenStrings.size(); ++o)
			{
				///for each children string
				///get the bone from my own skeleton's map that contains the childrenStrings[o]
				Bone* newChild = model->m_MeshSkeleton.GetBone(childrenStrings[o].c_str());
				///if it found something, add it to the BoneTemp children list and set its parent to BoneTemp which is
				///the bone i'm currently iterating over.
				if (newChild != NULL)
				{
					BoneTemp->AddBoneChildren(newChild);
					newChild->SetParent(BoneTemp);
				}
				else
				{
					Bone NoBone;
					NoBone.m_Name = childrenStrings[o];
					NoBone.m_JointMatrix.Identity();
					NoBone.m_OffsetMatrix.Identity();
					NoBone.m_AccumulatedTransform.Identity();
					NoBone.m_BoneIndex = BoneNames.size();
					NoBone.SetParent(BoneTemp);


					model->m_MeshSkeleton.AddBone(NoBone, childrenStrings[o]);
					BoneTemp->AddBoneChildren(&model->m_MeshSkeleton.m_Bones[NoBone.m_Name.c_str()]);
					BoneNames.push_back(NoBone.m_Name);
				}
			}

			Matrix4 matrice(INIT_UNIT);
			///Create a matrix for this bone in the final bone matrix vector
			model->m_MeshSkeleton.m_BoneFinalMatrixVector.push_back(matrice);
		}

		///Create a tempAnim pointer
		StrPtr<Animation> TempAnim = std::make_shared<Animation>();
		TempAnim->Load(filePath, "Anim");
		model->m_SetAnimation = rsc->InsertResource(TempAnim);

		///Create the vertex and index buffers
		model->m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, gfx);
		model->m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, gfx);

		///Create the Skeleton's buffers
		model->m_MeshSkeleton.InitSkeletonBuffer(gfx);

		return model;
	}

	bool AssimpLoader::AddNodeMesh(aiNode * pNode, const char * parent)
	{
		const char* nodeName = pNode->mName.C_Str();
		//FbxDouble3 translation = pNode->LclTranslation.Get();
		//FbxDouble3 rotation = pNode->LclRotation.Get();
		//FbxDouble3 scaling = pNode->LclScaling.Get();
		//
		//Vector3 trans(translation.mData[0], translation.mData[1], translation.mData[2]);
		//Vector3 rot(rotation.mData[0], rotation.mData[1], rotation.mData[2]);
		//Vector3 scale(scaling.mData[0], scaling.mData[1], scaling.mData[2]);

		//WeakGameObjectPtr node;
		//
		//if (!parent)
		//{
		//	node = SceneManager::Pointer()->GetActiveScene()->
		//		CreateActor(nodeName, NULL, trans, rot, scale);
		//}
		//else
		//{
		//	auto scene = SceneManager::Pointer()->GetActiveScene();
		//
		//	auto ptrParent = scene->FindActor(parent);
		//
		//	node = scene->
		//		CreateActor(nodeName, ptrParent.lock().get(), trans, rot, scale);
		//}


		return false;
	}

	void AssimpLoader::LoadMaterials(Map<String, ResourceHandle>& materialMap)
	{
		///Adds the materials
		for (uint32 k = 0; k < m_Scene->mNumMaterials; ++k)
		{
			aiMaterial* aimatTemp = m_Scene->mMaterials[k];

			///Get the material's name
			aiString name;
			aimatTemp->Get(AI_MATKEY_NAME, name);

			StrPtr<Material> Temp = std::make_shared<Material>();
			Temp->SetName(name.C_Str());

			ResourceHandle tempHandle = m_Res->InsertResource(Temp);

			materialMap.insert(Pair<String, ResourceHandle>(Temp->GetName(), tempHandle));

			Temp->InitMaterial(MAT_PBR_MetRough, Gfx::GfxCore::Pointer());
		}
	}
}