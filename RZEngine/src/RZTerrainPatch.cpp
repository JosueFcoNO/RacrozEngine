#include <RZEnginePCH.h>

namespace rczEngine
{
	void TerrainPatch::GeneratePatch(MeshPlane* left, MeshPlane * right, MeshPlane * down, MeshPlane * rightDown, Gfx::GfxCore* gfx)
	{
		GenerateVertexBuffer(left, right, down, rightDown);
		GenerateIndices();
		m_Mesh.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, gfx);
		m_Index.CreateIndexBuffer(Gfx::USAGE_DEFAULT, gfx);
	}

	void TerrainPatch::Render(Gfx::GfxCore * gfx)
	{
		m_Index.SetThisIndexBuffer(gfx);
		m_Mesh.SetThisVertexBuffer(gfx, 0);
		gfx->DrawIndexed(m_Index.GetSize(), 0, 0);
	}

	void TerrainPatch::LerpVertex(Gfx::Vertex * out_vertex, Gfx::Vertex * left, Gfx::Vertex * right, float amount)
	{
		out_vertex->VertexPosition = Math::Lerp(left->VertexPosition, right->VertexPosition, amount);
		out_vertex->VertexNormals = Math::Lerp(left->VertexNormals, right->VertexNormals, amount);
		out_vertex->TextureCoordinates = Math::Lerp(left->TextureCoordinates, right->TextureCoordinates, amount);
		out_vertex->Tangents = Math::Lerp(left->Tangents, right->Tangents, amount);
		out_vertex->BiNormals = Math::Lerp(left->BiNormals, right->BiNormals, amount);
	}

	void TerrainPatch::GenerateVertexBuffer(MeshPlane* left, MeshPlane * right, MeshPlane * down, MeshPlane * rightDown)
	{
		Gfx::Vertex TempVertex1, TempVertex2;

		int32 LeftSize = left->m_MeshBuffer.Size;
		int32 DownSize = down->m_MeshBuffer.Size;
		int32 RightSize = right->m_MeshBuffer.Size;

		int32 LeftMul = 512 / LeftSize;
		int32 RightMul = 512 / RightSize;
		int32 DownMul = 512 / DownSize;

		Gfx::Vertex* PrevVertex1 = NULL;
		Gfx::Vertex* PrevVertex2 = NULL;
		Gfx::Vertex* Next = NULL;

		for (int32 i = 0; i < 512; ++i)
		{
			////////////////////////////////////////////////////////
			//////////////////////LEFT//////////////////////////////
			////////////////////////////////////////////////////////

			int32 currentRow = i / LeftMul; ///Get the row for the vertex.

			if (i % LeftMul == 0) ///If there's a corresponding vertex.
			{
				PrevVertex1 = &left->m_VertexBuffer.GetVertex((currentRow+1) * (LeftSize)-1); ///Get the vertex and save it as the last corresponding vertex.
				PrevVertex1->VertexPosition.m_z -= 511.0f;
				PrevVertex1->TextureCoordinates.m_x = float(currentRow);
				PrevVertex1->TextureCoordinates.m_y = float(currentRow);

				TempVertex1 = *PrevVertex1; ///Create the new vertex.
			}
			else ///If there's no corresponding vertex, we create one interpolating.
			{
				Next = &left->m_VertexBuffer.GetVertex((currentRow + 1) * (LeftSize - 1));

				float LerpAmount = float(i) / float(LeftMul);
				LerpAmount -= currentRow;

				LerpVertex(&TempVertex1, PrevVertex1, Next, LerpAmount);
			}

			m_Mesh.AddVertex(TempVertex1); ///Save the vertex;

			////////////////////////////////////////////////////////
			////////////////////////////RIGHT///////////////////////
			////////////////////////////////////////////////////////

			currentRow = i / RightMul; ///Get the row for the vertex.

			if (i % RightMul == 0) ///If there's a corresponding vertex.
			{
				PrevVertex2 = &right->m_VertexBuffer.GetVertex((currentRow) * (RightSize)); ///Get the vertex and save it as the last corresponding vertex.
				PrevVertex2->VertexPosition.m_z = 1.0f;
				PrevVertex1->TextureCoordinates.m_x = 1.0f;

				TempVertex2 = *PrevVertex2; ///Create the new vertex.
			}
			else ///If there's no corresponding vertex, we create one interpolating.
			{
				Next = &right->m_VertexBuffer.GetVertex((currentRow + 1) * (RightSize));

				float LerpAmount = float(i) / float(RightMul);
				LerpAmount -= currentRow;
				LerpVertex(&TempVertex2, PrevVertex2, Next, Math::Clamp01(LerpAmount));

				TempVertex2.VertexPosition.m_z = 1.0f; ///Add the offset.

			}

			m_Mesh.AddVertex(TempVertex2); ///Save the vertex;
		}

		////////////////////////////////////////////////////////
		/////////////////////////////LEFT///////////////////////
		////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////
		/////////////////////////////LEFT///////////////////////
		////////////////////////////////////////////////////////

		//for (int32 i = 0; i < 512; ++i)
		//{
		//	////////////////////////////////////////////////////////
		//	/////////////////////////////LEFT///////////////////////
		//	////////////////////////////////////////////////////////

		//	int currentRow = i / LeftMul; ///Get the row for the vertex.

		//	if (i % LeftMul == 0) ///If there's a corresponding vertex.
		//	{
		//		PrevVertex1 = &left->m_VertexBuffer.GetVertex((currentRow)+(LeftSize - 1)*LeftSize); ///Get the vertex and save it as the last corresponding vertex.
		//		TempVertex1 = *PrevVertex1; ///Create the new vertex.
		//		m_Mesh.AddVertex(TempVertex1); ///Save the vertex;

		//	}
		//	else ///If there's no corresponding vertex, we create one interpolating.
		//	{
		//		Next = &left->m_VertexBuffer.GetVertex((currentRow + 1) + (LeftSize - 1)*LeftSize);

		//		float LerpAmount = float(i) / float(LeftMul);
		//		LerpAmount -= currentRow;

		//		LerpVertex(&TempVertex1, PrevVertex1, Next, LerpAmount);

		//		m_Mesh.AddVertex(TempVertex1); ///Save the vertex;
		//	}

		//	////////////////////////////////////////////////////////
		//	////////////////////////////DOWN////////////////////////
		//	////////////////////////////////////////////////////////

		//	currentRow = i / DownMul; ///Get the row for the vertex.

		//	if (i % DownMul == 0) ///If there's a corresponding vertex.
		//	{
		//		PrevVertex2 = &down->m_VertexBuffer.GetVertex(currentRow); ///Get the vertex and save it as the last corresponding vertex.
		//		TempVertex2 = *PrevVertex2; ///Create the new vertex.
		//		m_Mesh.AddVertex(TempVertex2); ///Save the vertex;

		//	}
		//	else ///If there's no corresponding vertex, we create one interpolating.
		//	{
		//		Next = &down->m_VertexBuffer.GetVertex((currentRow + 1));

		//		float LerpAmount = float(i) / float(RightMul);
		//		LerpAmount -= currentRow;

		//		LerpVertex(&TempVertex2, PrevVertex2, Next, LerpAmount);

		//		TempVertex2.VertexPosition.m_z -= 513.0f; ///Add the offset.

		//		m_Mesh.AddVertex(TempVertex2); ///Save the vertex;
		//	}

		//}
	}

	void TerrainPatch::GenerateIndices()
	{
		int32 currentIndex = 0;

		///Left To Right Triangles
		for (int32 i = 0; i < 511; ++i)
		{
			m_Index.AddIndex(currentIndex);
			m_Index.AddIndex(currentIndex + 1);
			m_Index.AddIndex(currentIndex + 2);

			m_Index.AddIndex(currentIndex + 1);
			m_Index.AddIndex(currentIndex + 3);
			m_Index.AddIndex(currentIndex + 2);

			currentIndex += 2;
		}

		//currentIndex -= 1;

		/////Up To Down Triangles
		//for (int32 i = 0; i < 511; ++i)
		//{
		//	m_Index.AddIndex(currentIndex);
		//	m_Index.AddIndex(currentIndex + 2);
		//	m_Index.AddIndex(currentIndex + 1);

		//	m_Index.AddIndex(currentIndex + 2);
		//	m_Index.AddIndex(currentIndex + 3);
		//	m_Index.AddIndex(currentIndex + 1);

		//	currentIndex += 1;
		//}
	}
}