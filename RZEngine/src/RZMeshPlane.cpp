#include "RZEnginePCH.h"

namespace rczEngine
{
	void MeshPlane::InitMeshPlane(int32 vertices, float size, ComputeAPI* compute, bool CreateIndexBuffer)
	{
		m_gfx = Gfx::GfxCore::Pointer();
		m_res = ResVault::Pointer();
		m_capi = compute;

		m_MeshBuffer.Size = vertices;
		m_MeshBuffer.distVertex = size / (vertices-1) + 0.0015f;
		m_MeshBuffer.Range = 1.0f;
		m_MeshBuffer.Octaves = 1;
		m_MeshBuffer.Persistance = 1.0f;

		Gfx::Vertex TempVertex;

		int SizeSquared = m_MeshBuffer.Size*m_MeshBuffer.Size;

#pragma omp parallel for
		for (int32 i = 0; i < SizeSquared; ++i)
		{
			m_VertexBuffer.AddVertex(TempVertex);
		}

		//m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DYNAMIC, false, m_gfx);

		if (CreateIndexBuffer)
		{
			m_IndexBuffer = new Gfx::IndexBuffer;
			MeshPlane::GenerateIndices(vertices, *m_IndexBuffer);
		}
		else
		{
			m_IndexBuffer = nullptr;
		}



		StrPtr<Texture2D> TexNoise = std::make_shared<Texture2D>();
		m_gfx->CreateTexture(m_NoisePatch, HEIGHTMAP_RES * sizeof(float), 1, TexNoise->m_TextureCore, HEIGHTMAP_RES, HEIGHTMAP_RES, Gfx::BIND_SHADER_RESOURCE, Gfx::FORMAT_R32_FLOAT);
		HeightMap = m_res->InsertResource(TexNoise);
		
		//Texture2D* TexNormal = new Texture2D;
		//m_gfx->CreateTexture(m_NormalTex, HEIGHTMAP_RES * sizeof(Vector3), 1, TexNormal->m_TextureCore, HEIGHTMAP_RES, HEIGHTMAP_RES, Gfx::BIND_SHADER_RESOURCE, Gfx::FORMAT_R32G32B32_FLOAT);
		//NormalMap = m_res->InsertResource(TexNormal);

	}

	void MeshPlane::RenderMeshPlane(uint32 size)
	{
		///Set the material and vertex buffer on the pipeline.
		m_res->GetResource<Material>(m_Material).lock()->SetThisMaterial(m_gfx, m_res);
		
		if (HeightMap)
		{
			m_res->GetResource<Texture2D>(HeightMap).lock()->SetThisTextureInDS(1, 1, m_gfx);
			m_res->GetResource<Texture2D>(HeightMap).lock()->SetThisTextureInPS(0, 1, m_gfx);
		}

		m_VertexBuffer.SetThisVertexBuffer(m_gfx);

		int32 vertexSize;

		if (m_IndexBuffer)
		{
			m_IndexBuffer->SetThisIndexBuffer(m_gfx);
			vertexSize = m_IndexBuffer->m_IndexSize;
		}
		else
		{
			vertexSize = size;
		}

		m_gfx->DrawIndexed(vertexSize, 0, 0);
	}

	void MeshPlane::GenerateIndices(int32 vertices, Gfx::IndexBuffer & indexBuffer)
	{
		indexBuffer.ClearIndexList();

		int32 index[3];
		int32 index2[3];

		index[0] = 1;
		index[1] = vertices;
		index[2] = 0;

		index2[0] = 1;
		index2[1] = vertices + 1;
		index2[2] = vertices;

		int32 row = 1;

		int32 indexNumber = 0;
		while (indexNumber != Math::Square(vertices - 1) * 6)
		{
			indexNumber += 6;

			indexBuffer.AddIndex(index[0]);
			indexBuffer.AddIndex(index[1]);
			indexBuffer.AddIndex(index[2]);

			indexBuffer.AddIndex(index2[0]);
			indexBuffer.AddIndex(index2[1]);
			indexBuffer.AddIndex(index2[2]);

			index[0] += 1;
			index[1] += 1;
			index[2] += 1;

			index2[0] += 1;
			index2[1] += 1;
			index2[2] += 1;

			if (index[0] == vertices*row)
			{
				index[0] += 1;
				index[1] += 1;
				index[2] += 1;


				index2[0] += 1;
				index2[1] += 1;
				index2[2] += 1;

				row++;
			}

			//if (index2[1] == vertices - 1)
			//{
			//	break;
			//}
		}

		//indexNumber += 3;

		auto indexnumber2 = indexNumber;

		while (indexnumber2 != Math::Square(vertices) * 6)
		{
			indexBuffer.AddIndex(0);
			indexnumber2++;
		}

		indexBuffer.m_IndexSize = indexNumber;
		indexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, Gfx::GfxCore::Pointer());
	}

	void MeshPlane::GenerateMesh()
	{
		Gfx::Vertex* TempVertex;
		auto size = m_MeshBuffer.Size;

#pragma omp parallel for
		for (uint32 i = 0; i < m_VertexBuffer.GetSize(); ++i)
		{
			TempVertex = &m_VertexBuffer.GetVertex(i);

			int32 x = i / size;
			int32 y = i % size;

			TempVertex->VertexPosition.m_x = float(m_MeshBuffer.distVertex*x);
			TempVertex->VertexPosition.m_y = 1.0f;
			TempVertex->VertexPosition.m_z = float(m_MeshBuffer.distVertex*y);

			TempVertex->TextureCoordinates.m_x = float(y) / m_MeshBuffer.Size;
			TempVertex->TextureCoordinates.m_y = float(x) / m_MeshBuffer.Size;
		}
	
		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, m_gfx);
	}

	void MeshPlane::GenerateNormals()
	{
		//Le saco normales a todo.
#pragma omp parallel for
		for (uint32 i = 0; i < m_IndexBuffer->GetSize(); i += 3)
		{
			auto index1 = m_IndexBuffer->GetIndex(i);
			auto index2 = m_IndexBuffer->GetIndex(i + 1);
			auto index3 = m_IndexBuffer->GetIndex(i + 2);

			Gfx::Vertex* Vert1 = &m_VertexBuffer.GetVertex(index1);
			Gfx::Vertex* Vert2 = &m_VertexBuffer.GetVertex(index2);
			Gfx::Vertex* Vert3 = &m_VertexBuffer.GetVertex(index3);

			Vector3 V1 = Vert1->VertexPosition - Vert2->VertexPosition;
			Vector3 V2 = Vert1->VertexPosition - Vert3->VertexPosition;

			V1.Normalize();
			V2.Normalize();

			Vert1->VertexNormals = Vert2->VertexNormals = Vert3->VertexNormals = (V1^V2).GetNormalized();
			Vert1->Tangents = Vert2->Tangents = Vert3->Tangents = (V1).GetNormalized();
			Vert1->BiNormals = Vert2->BiNormals = Vert3->BiNormals = (V2).GetNormalized();

		}
	}

	void MeshPlane::GenerateSmoothNormals()
	{
		auto size = m_MeshBuffer.Size;

		//Le saco normales suaves ya a todo.
#pragma omp parallel for
		for (uint32 i = 0; i < m_VertexBuffer.GetSize(); ++i)
		{
			Gfx::Vertex* ThisVertex = &m_VertexBuffer.GetVertex(i);

			int32 x = i % size;
			int32 y = i / size;

			Gfx::Vertex* NearbyVertices[8];
			int32 VerticesUsed = 0;

			if (x > 0)
			{
				NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i - 1);
				++VerticesUsed;
			}

			if (x < size)
			{
				NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i + 1);
				++VerticesUsed;
			}

			if (y > 0)
			{
				if (x > 0)
				{
					NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i - 1 - size);
					++VerticesUsed;
				}

				NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i - size);
				++VerticesUsed;

				if (x < size)
				{
					NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i + 1 - size);
					++VerticesUsed;
				}
			}

			if (y < size)
			{
				if (x > 0)
				{
					NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i - 1 + size);
					++VerticesUsed;
				}

				NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i + size);
				++VerticesUsed;

				if (x < size)
				{
					NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i + 1 + size);
					++VerticesUsed;
				}
			}

			Vector3 normalAvg(INIT_ZERO);
			Vector3 binormalAvg(INIT_ZERO);
			Vector3 TangentAvg(INIT_ZERO);

			for (int32 k = 0; k < VerticesUsed; ++k)
			{
				normalAvg += NearbyVertices[k]->VertexNormals;
				binormalAvg += NearbyVertices[k]->BiNormals;
				TangentAvg += NearbyVertices[k]->Tangents;

			}

			normalAvg   /= float(VerticesUsed + 1);
			binormalAvg /= float(VerticesUsed + 1);
			TangentAvg  /= float(VerticesUsed + 1);

			ThisVertex->VertexNormals = normalAvg;
			ThisVertex->BiNormals = binormalAvg;
			ThisVertex->Tangents = TangentAvg;
		}
	}
	

}