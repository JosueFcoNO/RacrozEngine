#include "RZEnginePCH.h"

namespace rczEngine
{

	void MeshPlane::InitMeshPlane(int32 vertices, double size, double HalfSize, Vector3 startPos, eMeshPlaneOrientation orientation, bool CreateIndexBuffer, bool CreateVertexBuffer)
	{
		m_gfx = Gfx::GfxCore::Pointer();
		m_res = ResVault::Pointer();
		m_capi = ComputeAPI::Pointer();

		m_MeshBuffer.Size = vertices;
		m_MeshBuffer.distVertex = size / double(vertices - 1);
		m_MeshBuffer.HalfSize = HalfSize;
		m_MeshBuffer.Range = 1.0f;
		m_MeshBuffer.Octaves = 1;
		m_MeshBuffer.Persistance = 1.0f;

		m_VertexBuffer.ResizeVertexVector(Math::Pow(m_MeshBuffer.Size, 2));

		if (CreateIndexBuffer)
		{
			m_IndexBuffer = new Gfx::IndexBuffer;
			MeshPlane::GenerateIndices(vertices, *m_IndexBuffer);
		}
		else
		{
			m_IndexBuffer = nullptr;
		}

		m_MeshAABB.SetAABB(Vector3(10, 10, 10), Vector3(-10, -10, -10));

		GenerateMesh(startPos, orientation);
		GenerateNormals();
		GenerateSmoothNormals();

		if (CreateVertexBuffer)
			m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DYNAMIC, false, m_gfx);

		m_StartPos = startPos;

	}

	void MeshPlane::DestroyMeshPlane() noexcept
	{
		m_VertexBuffer.Destroy();
		m_IndexBuffer->Destroy();
	}

	void MeshPlane::Render()
	{
		///Set the material and vertex buffer on the pipeline.
		//m_res->GetResource<Material>(m_Material).lock()->SetThisMaterial(m_gfx, m_res);

		m_VertexBuffer.SetThisVertexBuffer(m_gfx, 0);

		int32 vertexSize;

		if (m_IndexBuffer)
		{
			m_IndexBuffer->SetThisIndexBuffer(m_gfx);
			vertexSize = m_IndexBuffer->m_IndexSize;
		}
		else
		{
			vertexSize = m_MeshBuffer.Size;
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

	void MeshPlane::GenerateMesh(Vector3 startPos, eMeshPlaneOrientation orientation)
	{
		ProfilerObj obj("GenerateMesh", PROFILE_EVENTS::PROF_GAME);

		Gfx::Vertex* TempVertex;
		auto size = m_MeshBuffer.Size;
		auto vertexSize = m_VertexBuffer.GetSize();

		double Size = size * m_MeshBuffer.distVertex;
		double halfSize = m_MeshBuffer.HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = &m_VertexBuffer.GetVertex(i);

			int32 x = i / size;
			int32 y = i % size;

			switch (orientation)
			{
			case rczEngine::Ypos:
				TempVertex->VertexPosition.m_x = (m_MeshBuffer.distVertex*x) - halfSize;
				TempVertex->VertexPosition.m_y = 0.0f;
				TempVertex->VertexPosition.m_z = (m_MeshBuffer.distVertex*y) - halfSize;
				break;
			case rczEngine::Yneg:
				TempVertex->VertexPosition.m_x = CastStatic<float>(((m_MeshBuffer.distVertex*(size - y)) - halfSize) - m_MeshBuffer.distVertex);
				TempVertex->VertexPosition.m_y = 0.0f;
				TempVertex->VertexPosition.m_z = CastStatic<float>(((m_MeshBuffer.distVertex*(size - x)) - halfSize) - m_MeshBuffer.distVertex);
				break;
			case rczEngine::Xpos:
				TempVertex->VertexPosition.m_x = 0.0f;
				TempVertex->VertexPosition.m_y = CastStatic<float>((m_MeshBuffer.distVertex*y) - halfSize);
				TempVertex->VertexPosition.m_z = CastStatic<float>((m_MeshBuffer.distVertex*x) - halfSize);
				break;
			case rczEngine::Xneg:
				TempVertex->VertexPosition.m_x = 0.0f;
				TempVertex->VertexPosition.m_y = CastStatic<float>(((m_MeshBuffer.distVertex*(size - x)) - halfSize) - m_MeshBuffer.distVertex);
				TempVertex->VertexPosition.m_z = CastStatic<float>(((m_MeshBuffer.distVertex*(size - y)) - halfSize) - m_MeshBuffer.distVertex);
				break;
			case rczEngine::Zpos:
				TempVertex->VertexPosition.m_x = CastStatic<float>((m_MeshBuffer.distVertex*y) - halfSize);
				TempVertex->VertexPosition.m_y = CastStatic<float>((m_MeshBuffer.distVertex*x) - halfSize);
				TempVertex->VertexPosition.m_z = 0.0f;
				break;
			case rczEngine::Zneg:
				TempVertex->VertexPosition.m_x = CastStatic<float>(((m_MeshBuffer.distVertex*(size - x)) - halfSize) - m_MeshBuffer.distVertex);
				TempVertex->VertexPosition.m_y = CastStatic<float>(((m_MeshBuffer.distVertex*(size - y)) - halfSize) - m_MeshBuffer.distVertex);
				TempVertex->VertexPosition.m_z = 0.0f;
				break;

			}

			TempVertex->VertexPosition += startPos;

			TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition);

			//m_MeshAABB.AddPoint(TempVertex->VertexPosition);
			
			//TempVertex->TextureCoordinates.m_x = float(y)  * m_MeshBuffer.distVertex * 1000;
			//TempVertex->TextureCoordinates.m_y = float(x)  * m_MeshBuffer.distVertex * 1000;
		}
	}

	Gfx::Vertex & MeshPlane::GetVertex(int32 x, int32 y)
	{
		//int32 _x = Math::Clamp(x, 0, m_MeshBuffer.Size-1);
		//int32 _y = Math::Clamp(y, 0, m_MeshBuffer.Size-1);

		return m_VertexBuffer.GetVertex(m_MeshBuffer.Size * y + x);
	}

	Vector3 MeshPlane::CalculateVertexPos(Vector3 pos)
	{
		return pos;
	}

	void MeshPlane::GenerateNormals()
	{
		ProfilerObj obj("GenerateNormals", PROFILE_EVENTS::PROF_GAME);

		auto size = m_MeshBuffer.Size;
		auto indexSize = m_IndexBuffer->GetSize();

		//Le saco normales a todo.
#pragma omp parallel for
		for (uint32 i = 0; i < indexSize; i += 3)
		{
			auto index1 = m_IndexBuffer->GetIndex(i);
			auto index2 = m_IndexBuffer->GetIndex(i + 1);
			auto index3 = m_IndexBuffer->GetIndex(i + 2);

			Gfx::Vertex* Vert1 = &m_VertexBuffer.GetVertex(index1);
			Gfx::Vertex* Vert2 = &m_VertexBuffer.GetVertex(index2);
			Gfx::Vertex* Vert3 = &m_VertexBuffer.GetVertex(index3);

			Vector3 V1 = (Vert1->VertexPosition - Vert2->VertexPosition);
			Vector3 V2 = (Vert1->VertexPosition - Vert3->VertexPosition);

			Vert1->VertexNormals = Vert2->VertexNormals = Vert3->VertexNormals = (V1^V2);
			Vert1->Tangents = Vert2->Tangents = Vert3->Tangents = (V1);
			Vert1->BiNormals = Vert2->BiNormals = Vert3->BiNormals = (V2);
		}

		auto index1 = m_IndexBuffer->GetIndex(indexSize - 1);
		auto index2 = m_IndexBuffer->GetIndex(indexSize - 2);
		auto index3 = m_IndexBuffer->GetIndex(indexSize - 3);

		Gfx::Vertex* Vert1 = &m_VertexBuffer.GetVertex(index1);
		Gfx::Vertex* Vert2 = &m_VertexBuffer.GetVertex(index2);
		Gfx::Vertex* Vert3 = &m_VertexBuffer.GetVertex(index3);

		Vector3 V1 = (Vert1->VertexPosition - Vert2->VertexPosition);
		Vector3 V2 = (Vert1->VertexPosition - Vert3->VertexPosition);

		Vert1->VertexNormals = Vert2->VertexNormals = Vert3->VertexNormals = (V1^V2);
		Vert1->Tangents = Vert2->Tangents = Vert3->Tangents = (V1);
		Vert1->BiNormals = Vert2->BiNormals = Vert3->BiNormals = (V2);
	}

	void MeshPlane::GenerateSmoothNormals()
	{
		ProfilerObj obj("GenerateSmoothNormals", PROFILE_EVENTS::PROF_GAME);

		auto size = m_MeshBuffer.Size;
		auto bufferSize = m_VertexBuffer.GetSize();

		//Le saco normales suaves ya a todo.
		for (uint32 i = 0; i < bufferSize; ++i)
		{
			Gfx::Vertex* ThisVertex = &m_VertexBuffer.GetVertex(i);

			int32 x = i % size;
			int32 y = i / size;

			//if (x == 0) continue;
			//if (y == 0) continue;
			//if (x == size - 1) continue;
			//if (y == size - 1) continue;

			Gfx::Vertex* NearbyVertices[8];
			int32 VerticesUsed = 0;

			if (i + size > bufferSize)
			{
				continue;
			}

			if (x > 0)
			{
				NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i - 1);
				++VerticesUsed;
			}

			if (x < size-1)
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

				if (x < size-1)
				{
					NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i + 1 - size);
					++VerticesUsed;
				}
			}

			if (y < size-1)
			{
				if (x > 0)
				{
					NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i - 1 + size);
					++VerticesUsed;
				}

				NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i + size);
				++VerticesUsed;

				if (x < size-1)
				{
					NearbyVertices[VerticesUsed] = &m_VertexBuffer.GetVertex(i + 1 + size);
					++VerticesUsed;
				}
			}

			Vector3 normalAvg;
			Vector3 binormalAvg;
			Vector3 TangentAvg;

			for (int32 k = 0; k < VerticesUsed; ++k)
			{
				normalAvg += NearbyVertices[k]->VertexNormals;
				binormalAvg += NearbyVertices[k]->BiNormals;
				TangentAvg += NearbyVertices[k]->Tangents;
			}

			//normalAvg /= float(VerticesUsed + 1);
			//binormalAvg /= float(VerticesUsed + 1);
			//TangentAvg /= float(VerticesUsed + 1);

			ThisVertex->VertexNormals = normalAvg.GetNormalized();
			ThisVertex->BiNormals = binormalAvg.GetNormalized();
			ThisVertex->Tangents = TangentAvg.GetNormalized();
		}
	}


}