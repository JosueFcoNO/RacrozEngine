#pragma once

namespace rczEngine
{

	class RZ_EXP PlanetQuadTreeNode : public MeshPlane
	{
	public:
		virtual ~PlanetQuadTreeNode() { DestroyQuadTreeNode(); };

		void InitQuadTree(Planet* planetRef, PerlinNoise3D* noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation orientation);
		void Update(Vector3 playerPos);
		void DestroyQuadTreeNode() noexcept;

		bool TestIfInside(Vector3 pos);
		void CalculateLOD(Vector3 pos);

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

	private:
		void RenderChildren();
		void GenerateChild(int index, int depth);
		bool CheckChildrenReady();
		void SetChildrenReady(int indexOfChild, bool value);

		std::thread m_ChildThread[4];
		int32 m_QuadTreeDepth = 0;
		eMeshPlaneOrientation m_Side;

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