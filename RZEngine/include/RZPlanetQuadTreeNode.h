#pragma once

namespace rczEngine
{
	enum class eSide
	{
		Up,
		Right,
		Down,
		Left
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

	class PlanetQuadTreeNode;

	struct RZ_EXP NodeConnection
	{
		uint32 Hash;
		int32 Depth;
		PlanetQuadTreeNode* node;
		Vector3 Pos;
		eSide Side;
	};

	class RZ_EXP PlanetQuadTreeNode
	{
	public:
		virtual ~PlanetQuadTreeNode() { DestroyQuadTreeNode(); };

		void InitQuadTree(const StrPtr<Planet> planetRef, const StrPtr<PlanetQuadTreeNode>& parent, const StrPtr<PerlinNoise3D>& noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation side);

		void Update(Vector3 playerPos);
		void DestroyQuadTreeNode() noexcept;
		void Render();

		void GenerateMesh(eMeshPlaneOrientation side);
		void ConstructPlanesAndCorners();

		Vector3 CalculateVertexPos(const Vector3& pos, float& out_displacement);

		void CalculateLOD(const Vector3& pos);
		void SubdivideNode();

		void GenerateChildren();
		void DestroyChildren();

		bool TestIfInside(const Vector3& pos);
		void TestVisibility(const Frustum& camFrustum, Vector<PlanetQuadTreeNode*>& nodesToDraw);

		FORCEINLINE int32 GetQuadTreeDepth() const noexcept { return m_QuadTreeDepth; };
		FORCEINLINE TerrainVertex& GetVertex(int32 x, int32 y) { return m_VertexBuffer.GetVertex(Size * y + x); }
		FORCEINLINE void SetMeshDirty() noexcept { m_MeshDirty = true; };
		FORCEINLINE const AABB& GetAABB() const noexcept { return m_MeshAABB; };

		static const int Mesh_Res = 65;
		static const int Mesh_Row_Size = Mesh_Res - 1;
		static const int Mesh_Row_Half = (Mesh_Res / 2);

		StrPtr<Planet> PlanetOwner = nullptr;

	private:
		void GenerateMeshYPos(const Vector3 & startPos);
		void GenerateMeshYNeg(const Vector3 & startPos);
		void GenerateMeshXPos(const Vector3 & startPos);
		void GenerateMeshXNeg(const Vector3 & startPos);
		void GenerateMeshZPos(const Vector3 & startPos);
		void GenerateMeshZNeg(const Vector3 & startPos);
		void GenerateNormals();
		void GenerateSmoothNormals();

		void RenderChildren();

		void GenerateChild(int index, int depth);
		void SetChildrenReady(int indexOfChild, bool value);
		bool CheckChildrenReady();

		Gfx::VertexBuffer<TerrainVertex> m_VertexBuffer;
		Gfx::IndexBuffer m_IndexBuffer;

		Vector3 m_StartPos;
		Plane m_NodePlanes[5];

		float m_TimeTillDeath = 0.1f;
		Timer DeathTimer;

		bool m_ChildGenerated = false;

		std::thread m_ChildThread[9];
		int32 m_QuadTreeDepth = 0;
		eMeshPlaneOrientation m_Side;

		ResourceHandle m_Material;

		AABB m_MeshAABB;

		StrPtr<PlanetQuadTreeNode> Parent = nullptr;
		StrPtr<PlanetQuadTreeNode> Children[9];

		bool ChildrenReady[9];
		bool done = false;

		bool m_Subdivided = false;
		bool m_PlayerInside = false;
		bool m_MeshDirty = false;

		StrPtr<PerlinNoise3D> m_PlanetNoise;

		int32 Size;
		double distVertex;
		double HalfSize;

		int m_ChildNumber = 0;

		int m_ID = -1;
	};
};