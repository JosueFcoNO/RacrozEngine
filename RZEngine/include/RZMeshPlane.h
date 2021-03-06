#pragma once

namespace rczEngine
{
	class RZ_EXP Planet;
	class RZ_EXP SpaceManager;
	class RZ_EXP TerrainPatch;

	///The Vertex structure
	class RZ_EXP TerrainVertex
	{
	public:
		Vector3 VertexPosition;
		Vector2 TextureCoordinates;
		Vector3 VertexNormals;
		Vector3 Tangents;
		Vector3 BiNormals;
		float Displacement;
	};

	class RZ_EXP MeshPlane
	{
	public:
		virtual ~MeshPlane() { DestroyMeshPlane(); };

		void InitMeshPlane(int32 vertices, double size, double halfSize, Vector3 startPos, bool CreateIndexBuffer = true, bool CreateVertexBuffer = true);
		void DestroyMeshPlane() noexcept;

		virtual void Render();
		virtual Vector3 CalculateVertexPos(const Vector3& pos, float& out_displacement);

		FORCEINLINE void SetMaterial(ResourceHandle mat) { m_Material = mat; };
		FORCEINLINE WeakPtr<Material> GetMaterial() { return ResVault::Pointer()->GetResource<Material>(m_Material); };

		static void GenerateIndices(int32 vertices, Gfx::IndexBuffer& indexBuffer);

		TerrainVertex& GetVertex(int32 x, int32 y);

		Gfx::VertexBuffer<TerrainVertex> m_VertexBuffer;

		void GenerateMesh();

	protected:
		void GenerateNormals();
		void GenerateSmoothNormals();

		Gfx::IndexBuffer* m_IndexBuffer;
		ResourceHandle m_Material;
		
		int32 Size;
		double distVertex;
		double HalfSize;

		AABB m_MeshAABB;

		Vector3 m_StartPos;
	};
}


