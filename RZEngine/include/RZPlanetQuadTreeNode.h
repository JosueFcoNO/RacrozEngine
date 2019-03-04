#pragma once

namespace rczEngine
{
	enum class eSide
	{
		Up,
		Right,
		Down,
		Left,
		Up_Reverse = -1,
		Right_Reverse = -2,
		Down_Reverse = -3,
		Left_Reverse = -4,
		Null
	};

	enum class eCorner
	{
		TopLeft,
		TopRight,
		DownRight,
		DownLeft
	};

	struct ParentSidesData
	{
		String Hashes[4] = { "", "", "", "" };
	};

	class RZ_EXP PlanetQuadTreeNode : public MeshPlane
	{
	public:
		virtual ~PlanetQuadTreeNode() { DestroyQuadTreeNode(); };

		void InitQuadTree(Planet* planetRef, PerlinNoise3D* noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation orientation, ParentSidesData sideData);
		void Update(Vector3 playerPos);
		void DestroyQuadTreeNode() noexcept;

		bool TestIfAdyacent(PlanetQuadTreeNode* Node);
		bool TestIfInside(Vector3 pos);
		void CalculateLOD(Vector3 pos);

		void GetSide(eSide side, Vector<Gfx::Vertex*>& out_VertexList);

		eSide GetSideFromCorners(uint32 cornerOne, uint32 cornerTwo);

		bool FindClosest(const Vector3 pos, eCorner & corner);

		bool FindClosest(const Vector3 pos);

		static void Connect(PlanetQuadTreeNode* one, PlanetQuadTreeNode* two);

		virtual void Render();
		virtual Vector3 CalculateVertexPos(Vector3 pos);

		void GenerateChildren();
		void DestroyChildren();

		int32 GetQuadTreeDepth() { return m_QuadTreeDepth; };

		Plane Planes[5];
		Planet* PlanetOwner = nullptr;
		bool Override = false;
		bool ActiveTouch = false;

		static const int MESH_RES = 64;
		static const int MESH_ROW_SIZE = MESH_RES - 1;
		static const int MESH_ROW_HALF = (MESH_RES / 2) - 1;

		//static void Patch(PatchData patchData);
		static uint32 HashCorner(Vector3 v);

	private:
		void RenderChildren();
		void GenerateChild(int index, int depth);
		bool CheckChildrenReady();
		void SetChildrenReady(int indexOfChild, bool value);

		uint32 m_CornersID[4];

		std::thread Child[4];
		int32 m_QuadTreeDepth = 0;
		eMeshPlaneOrientation m_Side;
		ParentSidesData sideHashData;

		PlanetQuadTreeNode* Parent = nullptr;
		PlanetQuadTreeNode* Children[4];
		bool ChildrenReady[4];

		PerlinNoise3D* Noise;

		AABB aabb;

		float m_TimeTillDeath = 0.1f;
		Timer DeathTimer;

		bool m_ChildGenerated = false;
	};
}