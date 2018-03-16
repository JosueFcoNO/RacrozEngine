#include <RZEnginePCH.h>

namespace rczEngine
{
	void Model::DrawModel(Gfx::GfxCore* gfx, void* res, Map<String, ResourceHandle>* materialOverride, MATERIAL_TYPE matType)
	{
		//TODO: Pasarle el material override ya me hace sucio.
		ResVault* Res = (ResVault*)res;

		///Sets his own buffers
		m_VertexBuffer.SetThisVertexBuffer(gfx);
		m_IndexBuffer.SetThisIndexBuffer(gfx);

		auto materials = (materialOverride) ? materialOverride : &m_MaterialMap;

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

	void Model::Load(const char * fileName, const char* resName, bool addToResourceManager)
	{
		ResVault* rsc = ResVault::Pointer();
		Gfx::GfxCore* gfx = Gfx::GfxCore::Pointer();

		m_FilePath = fileName;
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
		const aiScene* Scene = B.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_FlipUVs
		);

		///Set the VertexOffset, IndexOffset and IndexCount local variables to 0. These help creating the meshes and managing a single index and a single vertex buffer.
		uint32 VertexOffset = 0;
		uint32 IndexOffset = 0;
		uint32 IndexCount = 0;

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

		m_IndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, gfx);
		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, true, gfx);
		return;
	}

}