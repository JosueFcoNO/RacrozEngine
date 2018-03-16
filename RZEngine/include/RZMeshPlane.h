#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Planet;
	class RZ_UTILITY_EXPORT SpaceManager;
	class RZ_UTILITY_EXPORT TerrainPatch;

#define HEIGHTMAP_RES 1024

	struct MeshPlaneBuffer
	{
		int32 Size;
		double distVertex;
		float xCell;
		float yCell;

		float Range;
		int32 Octaves;
		float Persistance;
		int32 CubeSide;

		float padding[3];
	};

	class RZ_UTILITY_EXPORT MeshPlane
	{
	public:
		void InitMeshPlane(int32 vertices, float size, ComputeAPI* compute, bool CreateIndexBuffer = true);
		void RenderMeshPlane(uint32 size = 0);

		FORCEINLINE void SetMaterial(ResourceHandle mat) { m_Material = mat; };
		FORCEINLINE WeakPtr<Material> GetMaterial() { return m_res->GetResource<Material>(m_Material); };

		float m_NoisePatch[HEIGHTMAP_RES][HEIGHTMAP_RES];
		float m_NormalTex[HEIGHTMAP_RES][HEIGHTMAP_RES];

		MeshPlaneBuffer m_MeshBuffer;

		static void GenerateIndices(int32 vertices, Gfx::IndexBuffer& indexBuffer);

		Gfx::VertexBuffer<Gfx::Vertex> m_VertexBuffer;

		ResourceHandle HeightMap = 0;
		ResourceHandle NormalMap = 0;

		bool m_Dirty = false;
		void GenerateMesh();

	private:
		void GenerateNormals();
		void GenerateSmoothNormals();

		Gfx::IndexBuffer* m_IndexBuffer;

		ResourceHandle m_Material;

		Gfx::GfxCore* m_gfx = NULL;
		ResVault* m_res = NULL;
		ComputeAPI* m_capi = NULL;

		friend TerrainPatch;
	};
}


