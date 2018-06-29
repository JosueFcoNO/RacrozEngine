#pragma once

///////////////////////////////////
//////////VERTEX BUFFER////////////
///////////////////////////////////

namespace rczEngine
{
	namespace Gfx
	{
		template <class t>
		class RZ_UTILITY_EXPORT VertexBuffer : public BasicBuffer ///A Vertex Buffer that hold a Vertex List and a Buffer.
		{
		public:
			virtual ~VertexBuffer() { ClearVertexList(); };

			///Creates a vertex buffer with the specified usage from the internal vertex list.
			bool CreateVertexBuffer(eBUFFER_USAGE usage, bool clearVertexList, GfxCore* gfx)
			{
				bool b = Create(sizeof(t), uint32(m_VertexList.size()), &m_VertexList[0], sizeof(t)* uint32(m_VertexList.size()), 1, usage, eBIND_FLAGS::BIND_VERTEX_BUFFER, gfx);

				if (clearVertexList)
				{
					ClearVertexList();
				}

				return b;
			};

			///Set this vertex buffer on the pipeline.
			void SetThisVertexBuffer(GfxCore* gfx, int slot)
			{
				gfx->SetVertexBuffer(*this, 1, slot, sizeof(t), 0);
			};

			///Add a vertex.
			void AddVertex(t newVertex)
			{
				m_VertexList.push_back(newVertex);
				m_VertexNumber = uint32(m_VertexList.size());
			};

			///Clears the Vertex List
			void ClearVertexList()
			{
				m_VertexList.clear();
			};

			///Slooow.
			void RefillVertexList()
			{
				auto gfx = Gfx::GfxCore::Pointer();

				t* vertexData = gfx->GetBufferData(this);

				for (int32 i = 0; i < m_NumOfElements; ++i)
				{
					AddVertex(vertexData[i]);
				}

				free(vertexData);
			}

			///Returns the number of vertices
			uint32 GetSize() { return m_VertexNumber; };
			uint32 GetMemSize() { return m_VertexNumber * sizeof(t); };

			///Returns a specified vertex
			t& GetVertex(uint32 index) { return m_VertexList[index]; };

			void* GetVectorPtr() { return &m_VertexList[0]; };

			void UpdateVertexBuffer(GfxCore * gfx)
			{
				gfx->UpdateSubResource(this, &m_VertexList[0], 0, 0);
			};

		private:
			///The vertex list
			Vector<t> m_VertexList;
			///The number of vertices
			uint32 m_VertexNumber;
		};
	}
};

