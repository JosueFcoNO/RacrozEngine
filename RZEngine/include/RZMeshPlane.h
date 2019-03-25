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
		Vector3 NormalAvg;
	};

	class RZ_EXP MeshPlane
	{
	public:
		virtual ~MeshPlane() { DestroyMeshPlane(); };

		void InitMeshPlane(int32 vertices, double size, double HalfSize, Vector3 startPos, eMeshPlaneOrientation orientation, bool CreateIndexBuffer = true, bool CreateVertexBuffer = true);
		void DestroyMeshPlane() noexcept;

		virtual void Render();
		virtual Vector3 CalculateVertexPos(const Vector3& pos, float& out_displacement);

		FORCEINLINE void SetMaterial(ResourceHandle mat) { m_Material = mat; };
		FORCEINLINE WeakPtr<Material> GetMaterial() { return ResVault::Pointer()->GetResource<Material>(m_Material); };

		MeshPlaneBuffer m_MeshBuffer;

		static void GenerateIndices(int32 vertices, Gfx::IndexBuffer& indexBuffer);

		TerrainVertex& GetVertex(int32 x, int32 y);

		Gfx::VertexBuffer<TerrainVertex> m_VertexBuffer;

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

		Gfx::IndexBuffer* m_IndexBuffer;
		ResourceHandle m_Material;

		AABB m_MeshAABB;

		Vector3 m_StartPos;
	};
}


