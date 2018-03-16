#include <RZEnginePCH.h>


namespace rczEngine
{
	void SkinnedModel::DrawModel(Gfx::GfxCore * gfx, void * res, Map<String, ResourceHandle>* materialOverride, MATERIAL_TYPE matType)
	{
		//TODO: Pasarle el material override ya me hace sucio.
		ResVault* Res = (ResVault*)res;

		///Sets his own buffers
		m_VertexBuffer.SetThisVertexBuffer(gfx);
		m_IndexBuffer.SetThisIndexBuffer(gfx);
		m_MeshSkeleton.SetSkeletonBuffers(gfx);

		auto materials = (materialOverride) ? materialOverride : &m_MaterialMap;

		///Goes through its meshes and draws them
		for (int i = 0; i < m_VectorMeshes.size(); i++)
		{
			auto mat = Res->GetResource<Material>(materials->at(m_VectorMeshes[i].m_Material)).lock();

			if (mat->m_MatType == matType | matType == MAT_ANY)
			{
				mat->SetThisMaterial(gfx, Res);
				m_VectorMeshes[i].Draw(gfx);
			}
		}
	}

	void SkinnedModel::Load(const char* filePath, const char* resName, bool addToResourceManager)
	{
		ResVault* rsc = ResVault::Pointer();
		Gfx::GfxCore* gfx = Gfx::GfxCore::Pointer();

		Vector<String> BoneNames;

		m_FilePath = filePath;
		m_Name = resName;

		if (addToResourceManager)
		{
			rsc->InsertResource(this);
		}
		else
		{
			m_Handle = INVALID_RESOURCE;
		}

		///Clear the index and vertex buffer's lists
		m_IndexBuffer.ClearIndexList();
		m_VertexBuffer.ClearVertexList();
		m_VectorMeshes.clear();

		///Create an Importer and Read the file in fileName
		Assimp::Importer B;
		const aiScene* Scene = B.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights);

		///Set the VertexOffset, IndexOffset and IndexCount local variables to 0. These help creating the meshes and managing a single index and a single vertex buffer.
		uint32 VertexOffset = 0;
		uint32 IndexOffset = 0;
		uint32 IndexCount = 0;

		Vector<aiBone*> BoneList;

		///Adds the materials
		int32 materialIndexOffset = 0;

		for (uint32 k = 0; k < Scene->mNumMaterials; ++k)
		{
			Material* Temp = new Material;
			aiMaterial* aimatTemp = Scene->mMaterials[k];

			aiColor3D Color = { 0,0,0 };
			aimatTemp->Get(AI_MATKEY_COLOR_DIFFUSE, Color);
			Temp->m_core.m_Diffuse.m_x = Color.r;
			Temp->m_core.m_Diffuse.m_y = Color.g;
			Temp->m_core.m_Diffuse.m_z = Color.b;

			aimatTemp->Get(AI_MATKEY_COLOR_AMBIENT, Color);
			Temp->m_core.m_Ambient.m_x = Color.r;
			Temp->m_core.m_Ambient.m_y = Color.g;
			Temp->m_core.m_Ambient.m_z = Color.b;

			aimatTemp->Get(AI_MATKEY_COLOR_SPECULAR, Color);
			Temp->m_core.m_Specular.m_x = Color.r;
			Temp->m_core.m_Specular.m_y = Color.g;
			Temp->m_core.m_Specular.m_z = Color.b;

			Temp->m_core.m_SpecularStrength = 0;
			aimatTemp->Get(AI_MATKEY_SHININESS_STRENGTH, Temp->m_core.m_SpecularStrength);

			///Get the material's name
			aiString name;
			aimatTemp->Get(AI_MATKEY_NAME, name);
			Temp->m_Name = name.C_Str();

			ResourceHandle tempHandle = rsc->InsertResource(Temp);

			m_MaterialMap.insert(Pair<String, ResourceHandle>(Temp->m_Name, tempHandle));

			if (k == 0)
			{
				materialIndexOffset = tempHandle;
			}

			Temp->InitMaterial(MAT_PBR_MetRough, gfx);
		}

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

				m_VertexBuffer.AddVertex(Temp);
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
						SkndVertex = &m_VertexBuffer.GetVertex(CurrentBone->mWeights[o].mVertexId + TempMesh.m_MeshCore.m_VertexOffset);

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

					m_IndexBuffer.AddIndex(Index);
				}
			}

			///Insert the meshes in the this model's meshes vector
			m_VectorMeshes.push_back(TempMesh);

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
			memcpy(&TempBone.m_OffsetMatrix, &CurrentBone->mOffsetMatrix, sizeof(Matrix4));

			///Add the Temp bone to the m_Skeleton's Bone Map
			m_MeshSkeleton.AddBone(TempBone, CurrentBone->mName.C_Str());
		}

		///Set the real root Node to its parent
		if (RealRootNode->mParent)
		{
			RealRootNode = RealRootNode->mParent;
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
		TempRoot.m_TransformMatrix.Identity();
		TempRoot.m_AccumulatedTransform.Identity();

		TempRoot.SetParent(NULL);
		TempRoot.m_BoneIndex = 0;
		m_MeshSkeleton.AddBone(TempRoot, TempRoot.m_Name);
		m_MeshSkeleton.m_RootBone = &m_MeshSkeleton.m_Bones[TempRoot.m_Name];
		BoneNames.push_back(TempRoot.m_Name);

		///Create the hierarchy in my own skeleton
		for (int32 i = 0; i < BoneNames.size(); ++i)
		{
			///Create a vector of strings to save all the children's names
			Vector<String> childrenStrings;
			///Create a Temp aiNode pointer to the scene node that has this bone's name.
			aiNode* Temp = Scene->mRootNode->FindNode(BoneNames[i].c_str());
			///Create a temp bone
			Bone* BoneTemp = &m_MeshSkeleton.m_Bones[BoneNames[i]];
			
			////Esto no tiene sentido, pero lo tenía cuando funcionaba bien.
			memcpy(&BoneTemp->m_JointMatrix, &Temp->mTransformation, sizeof(Matrix4));

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
							//BoneTemp->m_TransformMatrix = (Scale*Rotation*Translation*PreRotation);
							break;
						}
					}
				}
				else
				{
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
				Bone* newChild = m_MeshSkeleton.GetBone(childrenStrings[o].c_str());
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
					NoBone.m_BoneIndex = m_MeshSkeleton.m_Bones.size();
					NoBone.SetParent(BoneTemp);
					

					m_MeshSkeleton.AddBone(NoBone, childrenStrings[o]);
					BoneTemp->AddBoneChildren(&m_MeshSkeleton.m_Bones[NoBone.m_Name.c_str()]);
					BoneNames.push_back(NoBone.m_Name);
				}
			}

			Matrix4 matrice(INIT_UNIT);
			///Create a matrix for this bone in the final bone matrix vector
			m_MeshSkeleton.m_BoneFinalMatrixVector.push_back(matrice);
	}

		///Create a tempAnim pointer
		Animation* TempAnim = new Animation;
		TempAnim->Load(filePath, "Anim", false);
		m_SetAnimation = rsc->InsertResource(TempAnim);

		///Create the vertex and index buffers
		m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, gfx);
		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, gfx);

		///Create the Skeleton's buffers
		m_MeshSkeleton.InitSkeletonBuffer(gfx);

		return;
	}

	Vector<Animation*> SkinnedModel::LoadAnimations(char * filePath)
	{
		///Create a vector of Animation pointer to return.
		Vector<Animation*> returnVector;

		///Load the scene
		Assimp::Importer B;
		const aiScene* Scene = B.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

		///Create a tempAnim pointer
		Animation* TempAnim;

		///If the scene has animations
		if (Scene->HasAnimations())
		{
			///Iterate through the animations
			for (int32 i = 0; i < Scene->mNumAnimations; ++i)
			{
				///Allocate a new animation in TempAnim
				TempAnim = new Animation;

				///Set the duration, ticksPerSecond and CurrentTime
				aiAnimation* CurrentAnim = Scene->mAnimations[i];
				TempAnim->m_Duration = CurrentAnim->mDuration;
				TempAnim->m_TicksPerSecond = CurrentAnim->mTicksPerSecond;

				int o = 0;

				auto RootNameTrans = Parser::ParseToStrings<ANSICHAR>(CurrentAnim->mChannels[0]->mNodeName.data, "_", 0);
				auto RootNameRot = Parser::ParseToStrings<ANSICHAR>(CurrentAnim->mChannels[1]->mNodeName.data, "_", 0);
				auto RootNameScale = Parser::ParseToStrings<ANSICHAR>(CurrentAnim->mChannels[2]->mNodeName.data, "_", 0);

				BasicString<ANSICHAR> RootName = RootNameTrans[0];

				if (RootNameTrans[0] == RootNameScale[0] && RootNameTrans[0] == RootNameRot[0])
				{
					RootName = RootNameTrans[0];

					///Getting the Root Animation
					for (uint32 k = 0; k < CurrentAnim->mChannels[0]->mNumPositionKeys; ++k)
					{
						KeyFrame TempFrame;

						TempFrame.m_Time = CurrentAnim->mChannels[0]->mPositionKeys[k].mTime;

						TempFrame.m_Position.m_x = CurrentAnim->mChannels[0]->mPositionKeys[k].mValue.x;
						TempFrame.m_Position.m_y = CurrentAnim->mChannels[0]->mPositionKeys[k].mValue.y;
						TempFrame.m_Position.m_z = CurrentAnim->mChannels[0]->mPositionKeys[k].mValue.z;

						TempFrame.m_Scale.m_x = CurrentAnim->mChannels[2]->mScalingKeys[k].mValue.x;
						TempFrame.m_Scale.m_y = CurrentAnim->mChannels[2]->mScalingKeys[k].mValue.y;
						TempFrame.m_Scale.m_z = CurrentAnim->mChannels[2]->mScalingKeys[k].mValue.z;

						TempFrame.m_Rotation.m_x = CurrentAnim->mChannels[1]->mRotationKeys[k].mValue.x;
						TempFrame.m_Rotation.m_y = CurrentAnim->mChannels[1]->mRotationKeys[k].mValue.y;
						TempFrame.m_Rotation.m_z = CurrentAnim->mChannels[1]->mRotationKeys[k].mValue.z;
						TempFrame.m_Rotation.m_w = CurrentAnim->mChannels[1]->mRotationKeys[k].mValue.w;

						TempAnim->AddKeyFrame(TempFrame, RootName);
					}

					o = 3;
				}

				for (; o < CurrentAnim->mNumChannels; ++o)
				{
					CurrentAnim->mChannels[o]->mNodeName;

					auto NodeName = Parser::ParseToStrings<ANSICHAR>(CurrentAnim->mChannels[o]->mNodeName.data, "_", 0);
					BasicString<ANSICHAR> CurrentAnimNode = CurrentAnim->mChannels[o]->mNodeName.data;
					BasicString<ANSICHAR> CurrentFbxTransform = "";

					if (NodeName.size() > 2)
					{
						if (NodeName[1] == "$AssimpFbx$")
						{
							CurrentAnimNode = NodeName[0];
							CurrentFbxTransform = NodeName[2];
						}
					}

					///Go through the tracks of this current animation
					for (uint32 k = 0; k < CurrentAnim->mChannels[o]->mNumPositionKeys; ++k)
					{
						///Create a temp keyframe and set it with the current key frame data.
						KeyFrame TempFrame;
						TempFrame.m_Scale.Set(1, 1, 1);
						auto TempChannel = CurrentAnim->mChannels[o];

						TempFrame.m_Time = TempChannel->mPositionKeys[k].mTime;

						if (CurrentFbxTransform != "Rotation" && CurrentFbxTransform != "Scaling")
						{
							TempFrame.m_Position.m_x = TempChannel->mPositionKeys[k].mValue.x;
							TempFrame.m_Position.m_y = TempChannel->mPositionKeys[k].mValue.y;
							TempFrame.m_Position.m_z = TempChannel->mPositionKeys[k].mValue.z;
						}

						if (CurrentFbxTransform != "Rotation" && CurrentFbxTransform != "Translation")
						{
							TempFrame.m_Scale.m_x = TempChannel->mScalingKeys[k].mValue.x;
							TempFrame.m_Scale.m_y = TempChannel->mScalingKeys[k].mValue.y;
							TempFrame.m_Scale.m_z = TempChannel->mScalingKeys[k].mValue.z;
						}

						if (CurrentFbxTransform != "Scaling" && CurrentFbxTransform != "Translation")
						{
							TempFrame.m_Rotation.m_x = TempChannel->mRotationKeys[k].mValue.x;
							TempFrame.m_Rotation.m_y = TempChannel->mRotationKeys[k].mValue.y;
							TempFrame.m_Rotation.m_z = TempChannel->mRotationKeys[k].mValue.z;
							TempFrame.m_Rotation.m_w = TempChannel->mRotationKeys[k].mValue.w;
						}

						///Add it to the TempAnim in the respective track
						TempAnim->AddKeyFrame(TempFrame, CurrentAnimNode.data());
					}
				}

				///Push back the animation in the vector of animations
				returnVector.push_back(TempAnim);
			}
		}

		///Return the vector of animations
		return returnVector;
	}

}