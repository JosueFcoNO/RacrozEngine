#include <RZEnginePCH.h>

namespace rczEngine
{	
	Mesh::Mesh(Gfx::MeshAPI meshCore)
	{
		m_MeshCore = meshCore;
	}

	void Mesh::Draw(Gfx::GfxCore* gfx)
	{
		m_MeshCore.Draw(gfx);
	}

	void Mesh::Serialize()
	{
		auto ser = Serializer::Pointer();
		ser->SetNextObjectSerial(SERIAL_MESH);

		//Save the name
		ser->SerializeString(m_Name);

		//Save the material name
		ser->SerializeString(m_Material);

		//Save the vertex offset
		ser->WriteData(&m_MeshCore.m_VertexOffset, sizeof(uint32));

		//Save the index offset
		ser->WriteData(&m_MeshCore.m_IndexOffset, sizeof(uint32));

		//Save the index count
		ser->WriteData(&m_MeshCore.m_IndexCount, sizeof(uint32));
	}

	void Mesh::DeSerialize()
	{
		auto ser = Serializer::Pointer();

		ser->GetNextObjectSerial();

		//Save the name
		ser->DeSerializeString(m_Name);

		//Save the material name
		ser->DeSerializeString(m_Material);

		//Save the vertex offset
		ser->ReadData(&m_MeshCore.m_VertexOffset, sizeof(uint32));

		//Save the index offset
		ser->ReadData(&m_MeshCore.m_IndexOffset, sizeof(uint32));

		//Save the index count
		ser->ReadData(&m_MeshCore.m_IndexCount, sizeof(uint32));
	}

}