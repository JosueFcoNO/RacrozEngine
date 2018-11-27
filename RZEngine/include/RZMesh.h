#pragma once

namespace rczEngine
{
	////////////////MESH
	///A class Mesh that contains the material name of the mesh. 
	class RZ_EXP Mesh : public Serializable
	{
	public:
		Mesh() {};
		Mesh(Gfx::MeshAPI meshCore);

		void Draw(Gfx::GfxCore* gfx);

		virtual void Serialize();
		virtual void DeSerialize();

		Gfx::MeshAPI m_MeshCore;
		
		String m_Material;
		String m_Name;
	};
}