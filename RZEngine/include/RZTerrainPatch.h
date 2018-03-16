#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT TerrainPatch
	{
	public:
		void GeneratePatch(MeshPlane* left, MeshPlane* right, MeshPlane* down, MeshPlane* rightDown, Gfx::GfxCore* gfx);
		void Render(Gfx::GfxCore* gfx, ResVault* res);

	private:
		Gfx::VertexBuffer<Gfx::Vertex> m_Mesh;
		Gfx::IndexBuffer m_Index;

		void LerpVertex(Gfx::Vertex* out_vertex, Gfx::Vertex* left, Gfx::Vertex* right, float amount);
		void GenerateVertexBuffer(MeshPlane* left, MeshPlane * right, MeshPlane * down, MeshPlane * rightDown);
		void GenerateIndices();
	};
}