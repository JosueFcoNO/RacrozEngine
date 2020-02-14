#include <RZEnginePCH.h>

namespace rczEngine
{
	void Model::DrawModel(Map<String, ResourceHandle>* materialOverride, int renderHash)
	{
		//TODO: Pasarle el material override ya me hace sucio o no?
		auto res = ResVault::Pointer();
		auto gfx = Gfx::GfxCore::Pointer();

		///Sets his own buffers
		m_VertexBuffer.SetThisVertexBuffer(gfx, 0);
		m_IndexBuffer.SetThisIndexBuffer(gfx);

		auto materials = (materialOverride) ? materialOverride : &m_MaterialMap;

		for (int i = 0; i < m_VectorMeshes.size(); i++)
		{
			auto mat = res->GetResource<Material>(materials->at(m_VectorMeshes[i].m_Material)).lock();

			MaterialRenderInfo currentRenderInfo;
			currentRenderInfo.InitFromMaterial(*mat);
			currentRenderInfo.componentID = CMP_MODEL_RENDERER;
			auto currentRenderHash = MaterialRenderInfo::CalculateRenderHash(currentRenderInfo);

			if (currentRenderHash == renderHash || renderHash == -1)
			{
				if (mat->m_MatType != eMaterialType::PlanetSurface)
					mat->SetThisMaterial();

				m_VectorMeshes[i].Draw(gfx);
			}
		}
	}

	void Model::Serialize()
	{
		auto ser = Serializer::Pointer();
		auto gfx = Gfx::GfxCore::Pointer();
		ser->SetNextObjectSerial(SERIAL_MODEL);

		ser->SerializeString(m_Name);

		m_FilePath.Serialize();

		//Write the number of vertices.
		int32 tempInt = m_VertexBuffer.m_NumOfElements;
		ser->WriteData(&tempInt, sizeof(int32));

		//Write the vertices.
		void* ptr = gfx->GetBufferData(m_VertexBuffer);
		ser->WriteData(ptr, m_VertexBuffer.m_NumOfElements*m_VertexBuffer.m_SizeOfElement);
		free(ptr);

		//Write the number of indices.
		tempInt = m_IndexBuffer.m_NumOfElements;
		ser->WriteData(&tempInt, sizeof(int32));

		//Write the indices.
		ptr = gfx->GetBufferData(m_IndexBuffer);
		ser->WriteData(ptr, m_IndexBuffer.m_NumOfElements*m_IndexBuffer.m_SizeOfElement);
		free(ptr);

		//Write the number of meshes
		tempInt = (int32)m_VectorMeshes.size();
		ser->WriteData(&tempInt, sizeof(int32));

		//Write the meshes.
		for (int32 i = 0; i < tempInt; ++i)
		{
			m_VectorMeshes[i].Serialize();
		}

		//Write the number of materials in the map
		tempInt = (int32)m_MaterialMap.size();
		ser->WriteData(&tempInt, sizeof(int32));

		//Write the materials name and the handle.
		for (auto it = m_MaterialMap.begin(); it != m_MaterialMap.end(); ++it)
		{
			//Serialize the material name.
			ser->SerializeString(it->first);
			//Serialize the resource handle.
			ser->WriteData(&it->second, sizeof(ResourceHandle));
		}

	}

	void Model::DeSerialize()
	{
		auto ser = Serializer::Pointer();
		auto gfx = Gfx::GfxCore::Pointer();

		ser->DeSerializeString(m_Name);

		m_FilePath.DeSerialize();

		//Read the number of vertices.
		int32 tempInt;
		ser->ReadData(&tempInt, sizeof(int32));

		Gfx::Vertex TempVertex;
		//Fill the vertices vector.
		for (int32 i = 0; i < tempInt; ++i)
		{
			m_VertexBuffer.AddVertex(TempVertex);
		}

		//Read and overwrite the vertex buffers size;
		ser->ReadData(m_VertexBuffer.GetVectorPtr(), tempInt * sizeof(Gfx::Vertex));

		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, true, gfx);

		//Read the number of indices.
		ser->ReadData(&tempInt, sizeof(int32));

		//Fill the vertices vector.
		for (int32 i = 0; i < tempInt; ++i)
		{
			m_IndexBuffer.AddIndex(0);
		}

		//Read and overwrite the index buffers size;
		ser->ReadData(m_IndexBuffer.GetVectorStart(), tempInt * sizeof(uint32));

		m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, false, gfx);

		//Read the number of meshes
		ser->ReadData(&tempInt, sizeof(int32));

		Mesh tempMesh;
		//Read the meshes.
		for (int32 i = 0; i < tempInt; ++i)
		{
			tempMesh.DeSerialize();
			m_VectorMeshes.push_back(tempMesh);
		}

		//Read the number of materials in the map
		ser->ReadData(&tempInt, sizeof(int32));

		Pair<String, ResourceHandle> TempPair;
		for (int32 i = 0; i < tempInt; ++i)
		{
			//Read the string of the material name.
			ser->DeSerializeString(TempPair.first);
			//Read the resource handle
			ser->ReadData(&TempPair.second, sizeof(ResourceHandle));
			//Insert it to the material map.
			m_MaterialMap.insert(TempPair);
		}
	}

	void Model::Load(const String& fileName, const String& resName)
	{
		ResVault* rsc = ResVault::Pointer();
		Gfx::GfxCore* gfx = Gfx::GfxCore::Pointer();

		m_FilePath = fileName;
		m_Name = resName;

		rsc->InsertResource(shared_from_this());

		///Clear the index and vertex buffer's lists
		m_IndexBuffer.ClearIndexList();
		m_VertexBuffer.ClearVertexList();
		m_VectorMeshes.clear();

		///Create an Importer and Read the file in fileName
		Assimp::Importer B;
		const aiScene* Scene = B.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_FlipUVs
		);

		///Set the VertexOffset, IndexOffset and IndexCount local variables to 0. These help creating the meshes and managing a single index and a single vertex buffer.
		uint32 VertexOffset = 0;
		uint32 IndexOffset = 0;
		uint32 IndexCount = 0;

		for (uint32 k = 0; k < Scene->mNumMaterials; ++k)
		{
			aiMaterial* aimatTemp = Scene->mMaterials[k];

			///Get the material's name
			aiString name;
			aimatTemp->Get(AI_MATKEY_NAME, name);

			StrPtr<Material> Temp = std::make_shared<Material>();
			Temp->SetName(name.C_Str());
			Temp->SetFilePath(fileName + String(" | ") + name.C_Str());

			ResourceHandle tempHandle = rsc->InsertResource(Temp);

			m_MaterialMap.insert(Pair<String, ResourceHandle>(Temp->GetName(), tempHandle));

			Temp->InitMaterial(eMaterialType::PBR_MetRough);
		}

		///Iterate the scene's meshes.
		for (uint32 n = 0; n < Scene->mNumMeshes; ++n)
		{
			///CurrentMesh = the mesh i'm currently iterating over.
			aiMesh* CurrentMesh = Scene->mMeshes[n];

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
			Scene->mMaterials[CurrentMesh->mMaterialIndex]->Get(AI_MATKEY_NAME, matName);
			TempMesh.m_Material = matName.C_Str();

			///Insert the meshes in the this model's meshes vector
			m_VectorMeshes.push_back(TempMesh);

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

				m_VertexBuffer.AddVertex(Temp);
			}

			for (uint32 o = 0; o < CurrentMesh->mNumFaces; ++o)
			{
				for (uint32 i = 0; i < CurrentMesh->mFaces[o].mNumIndices; ++i)
				{
					uint32 Index;
					Index = CurrentMesh->mFaces[o].mIndices[i];

					m_IndexBuffer.AddIndex(Index);
				}
			}
		}

		m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, false, gfx);
		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, true, gfx);
		return;
	}

}