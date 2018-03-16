#pragma once

namespace rczEngine
{
	////////////////MESH
	///A class Mesh that contains Gfx::Vertex and Index buffers.
	class RZ_UTILITY_EXPORT SkinnedMesh
	{
	public:
		SkinnedMesh() {};
		SkinnedMesh(Gfx::MeshAPI meshCore);

		virtual ~SkinnedMesh() { Destroy(); };

		void Destroy() {};
		void Draw(Gfx::GfxCore * gfx);

		Gfx::MeshAPI m_MeshCore;

		String m_Material;
		String m_Name;
	};
}