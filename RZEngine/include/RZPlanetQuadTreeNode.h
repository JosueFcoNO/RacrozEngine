#pragma once

namespace rczEngine
{
	class RZ_EXP PlanetQuadTreeNode : public MeshPlane
	{
	public:
		virtual ~PlanetQuadTreeNode() { DestroyQuadTreeNode(); };

		void InitQuadTree(PerlinNoise3D* noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation orientation);
		void SetAdyacents(PlanetQuadTreeNode* Left, PlanetQuadTreeNode* Right, PlanetQuadTreeNode* Up, PlanetQuadTreeNode* Down);
		void Update(Vector3 playerPos);
		void DestroyQuadTreeNode() noexcept;

		void Test(Vector3 pos);

		virtual void Render();
		virtual Vector3 CalculateVertexPos(Vector3 pos);

		void GenerateChildren();
		void DestroyChildren();

		Plane Planes[5];
		Timer AdyTimer;

		bool Override = false;
		bool ActiveTouch = false;
		bool IsAdy = false;
	private:
		void RenderChildren();
		void GenerateChild(int index, int depth);
		bool CheckChildrenReady();
		void SetChildrenReady(int indexOfChild, bool value);

		std::thread Child[4];
		int32 m_QuadTreeDepth = 0;
		eMeshPlaneOrientation m_Side;

		PlanetQuadTreeNode* Parent;
		PlanetQuadTreeNode* Children[4];
		PlanetQuadTreeNode* Adyacents[4];
		bool ChildrenReady[4];

		PerlinNoise3D* Noise;

		AABB aabb;

		float m_TimeTillDeath = 0.1f;
		Timer DeathTimer;

		bool m_ChildGenerated = false;
	};
}