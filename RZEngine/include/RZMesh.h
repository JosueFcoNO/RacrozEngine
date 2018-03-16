#pragma once

namespace rczEngine
{
	////////////////MESH
	///A class Mesh that contains Gfx::Vertex and Index buffers.
	class RZ_UTILITY_EXPORT Mesh
	{
	public:
		Mesh() {};
		Mesh(Gfx::MeshAPI meshCore);

		virtual ~Mesh() { Destroy(); };

		void Destroy() {};
		void Draw(Gfx::GfxCore* gfx);

		Gfx::MeshAPI m_MeshCore;
		
		String m_Material;
		String m_Name;
	};
}