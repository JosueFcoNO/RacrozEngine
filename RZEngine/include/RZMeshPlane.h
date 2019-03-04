#pragma once

namespace rczEngine
{
	class RZ_EXP Planet;
	class RZ_EXP SpaceManager;
	class RZ_EXP TerrainPatch;

#define HEIGHTMAP_RES 1024

	struct MeshPlaneBuffer
	{
		int32 Size;
		double distVertex;
		double HalfSize;
		float xCell;
		float yCell;

		float Range;
		int32 Octaves;
		float Persistance;
		int32 CubeSide;

		float padding[3];
	};

	enum eMeshPlaneOrientation
	{
		Ypos,
		Yneg,
		Xpos,
		Xneg,
		Zpos,
		Zneg,
		Sides_Max
	};

	class RZ_EXP MeshPlane
	{
	public:
		virtual ~MeshPlane() { DestroyMeshPlane(); };

		void InitMeshPlane(int32 vertices, double size, double HalfSize, Vector3 startPos, eMeshPlaneOrientation orientation, bool CreateIndexBuffer = true, bool CreateVertexBuffer = true);
		void DestroyMeshPlane() noexcept;

		virtual void Render();
		virtual Vector3 CalculateVertexPos(Vector3 pos);

		FORCEINLINE void SetMaterial(ResourceHandle mat) { m_Material = mat; };
		FORCEINLINE WeakPtr<Material> GetMaterial() { return m_res->GetResource<Material>(m_Material); };

		MeshPlaneBuffer m_MeshBuffer;

		static void GenerateIndices(int32 vertices, Gfx::IndexBuffer& indexBuffer);

		Gfx::Vertex& GetVertex(int32 x, int32 y);

		Gfx::VertexBuffer<Gfx::Vertex> m_VertexBuffer;

		bool m_Dirty = false;

		void GenerateMesh(const Vector3& startPos, eMeshPlaneOrientation orientation);

	protected:
		void GenerateNormals();
		void GenerateSmoothNormals();

		void GenerateMeshYPos(const Vector3 & startPos);
		void GenerateMeshYNeg(const Vector3 & startPos);
		void GenerateMeshXPos(const Vector3 & startPos);
		void GenerateMeshXNeg(const Vector3 & startPos);
		void GenerateMeshZPos(const Vector3 & startPos);
		void GenerateMeshZNeg(const Vector3 & startPos);

		void FixBorders();

		Gfx::IndexBuffer* m_IndexBuffer;

		ResourceHandle m_Material;

		AABB m_MeshAABB;

		Gfx::GfxCore* m_gfx = NULL;
		ResVault* m_res = NULL;
		ComputeAPI* m_capi = NULL;

		Vector3 m_StartPos;

		friend TerrainPatch;
	};
}


