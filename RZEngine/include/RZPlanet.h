#pragma once
#define PERMUTATION_TABLE_SIZE 256

namespace rczEngine
{
	class RZ_EXP SpaceManager;

	class RZ_EXP PlanetVertex
	{
		Vector3 VertexPosition;
		Vector2 TextureCoordinates;
		Vector3 VertexNormals;
		Vector3 Tangents;
		Vector3 BiNormals;
		int Gradient[4];
	};

	class RZ_EXP PatchData
	{
	public:
		Vector<NodeConnection*> Connectors;
	};

	struct AtmosData
	{
		int nSamples = 4;
		float fSamples = 4.0f;

		float SkyScale = 2.5f;
		float OuterRadius = 102.5f;

		Vector3 InvWaveLength = { 5.602, 9.478, 19.646 };
		float OuterRadiusSquared = pow(OuterRadius, 2.0f);

		float InnerRadius = 100.0f;
		float InnerRadiusSquared = pow(InnerRadius, 2.0f);
		float KrESun = .0375;
		float KmESun = .0225;

		float Kr4PI = .0314;
		float Km4PI = .0188;
		float Scale = 1.0f / (OuterRadius - InnerRadius);
		float ScaleDepth = .25;

		float InvScaleDepth = 1.0f / ScaleDepth;
		float ScaleOverScaleDepth = Scale / ScaleDepth;
		float G = -.95;
		float GSquared = .9025;
	};

	class RZ_EXP Planet
	{ 
	public:
		PlanetQuadTreeNode* ActiveQuadTree = nullptr;

		void InitPlanet(int32 seed, float x, float y, float z, SpaceManager* spaceMng);
		void RenderPlanet(float scale);
		void RenderPlanetWater(float scale);
		void RenderAtmosphere();

		void CreateMaterial();

		void CreateNewNode();


		FORCEINLINE Vector3 GetSpacePosition() { return m_SpacePosition; };

		ResourceHandle m_Materials;
		ResourceHandle m_HeightMap;
		Vector4 m_HeightScale;

		int32 Seed = 0;
		PerlinNoise3D noise;

		AtmosData m_Atmosphere;

		Map<uint32, PatchData> m_PatchInfo;

		void NewQuad() { m_NodesCount++; Logger::Pointer()->Log("Node Count: " + std::to_string(m_NodesCount)); }

	private:
		int m_NodesCount = 6;

		void LoadAndProcessModel();
		void ProcessConnectionNode(NodeConnection& node);

		bool m_OnLand = false;

		StrPtr<Model> m_Planet;

		SpaceManager* m_SpaceMng;
		Gfx::GfxCore* m_GfxCore;
		StrPtr<Scene> m_CurrentScene;

		Gfx::ConstantBuffer m_GradientCB;
		Gfx::ConstantBuffer m_GradientsReal;
		Gfx::ConstantBuffer m_ScaleCB;

		Gfx::ConstantBuffer m_PosView;
		Vector4 PlanetPos;

		Gfx::ConstantBuffer m_HeightCameracb;
		Vector4 m_HeightCamera;
		
		Vector4 PermutationTable[PERMUTATION_TABLE_SIZE];
		Vector3 m_SpacePosition;
		Matrix4 m_PlanetMatrix;

		Vector<StrPtr<PlanetQuadTreeNode>> m_Nodes;
		PlanetQuadTreeNode m_QTreeRoots[6];

		ResourceHandle Water;
		StrPtr<CameraCmp> PlayerCamera;
	};
}