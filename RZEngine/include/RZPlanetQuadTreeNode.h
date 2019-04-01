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

	enum class ePointQNode
	{
		UpLeft,
		UpCenter,
		UpRight,
		RightCenter,
		DownRight,
		DownCenter,
		DownLeft,
		LeftCenter
	};

	class PlanetQuadTreeNode;

	struct RZ_EXP NodeConnection
	{
		uint32 Hash;
		int32 Depth;
		PlanetQuadTreeNode* node;
		Vector3 Pos;
		eSide Side;
		ePointQNode QPoint;
		bool Connector; 
	};

	class RZ_EXP PlanetQuadTreeNode : public MeshPlane
	{
	public:
		virtual ~PlanetQuadTreeNode() { DestroyQuadTreeNode(); };

		void InitQuadTree(Planet * planetRef, PlanetQuadTreeNode * parent, PerlinNoise3D * noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation side);
		void Update(Vector3 playerPos);
		void DestroyQuadTreeNode() noexcept;

		void InitPlaneAndCorners();

		bool TestIfInside(const Vector3& pos);
		void CalculateLOD(const Vector3& pos);

		void TestVisibility(const Frustum& camFrustum, Vector<PlanetQuadTreeNode*>& nodesToDraw);

		static void ConnectNodesSameDepth(const NodeConnection& one, const NodeConnection& two);

		virtual void Render();
		Vector3 CalculateVertexPos(const Vector3& pos, float& out_displacement) override;

		void GenerateChildren();
		void DestroyChildren();

		int32 GetQuadTreeDepth() { return m_QuadTreeDepth; };
		bool done = false;
		Plane Planes[5];
		Planet* PlanetOwner = nullptr;
		bool Override = false;
		bool ActiveTouch = false;
		bool m_MeshDirty = false;

		static const int MESH_RES = 64;
		static const int MESH_ROW_SIZE = MESH_RES - 1;
		static const int MESH_ROW_HALF = (MESH_RES / 2);

		Vector<NodeConnection> Connections;

	private:
		void RenderChildren();
		void GenerateChild(int index, int depth);
		bool CheckChildrenReady();
		void UpdateSideVertices();
		void SetChildrenReady(int indexOfChild, bool value);

		static uint32 HashCorner(Vector3 v);

		std::thread m_ChildThread[4];
		int32 m_QuadTreeDepth = 0;
		eMeshPlaneOrientation m_Side;

		Vector<TerrainVertex*> SideVertices[4];

		PlanetQuadTreeNode* Parent = nullptr;
		PlanetQuadTreeNode* Children[4];
		bool ChildrenReady[4];

		PerlinNoise3D* Noise;

		WeakPtr<DebuggerLineList> AABB_Debug;

		float m_TimeTillDeath = 0.1f;
		Timer DeathTimer;

		bool m_ChildGenerated = false;
	};
}