#pragma once

#define TERRAIN_NUMBER 13
#define RADIO 70.0f
#define RADIO_ATMOSFERA 2.5f

namespace rczEngine
{
	enum eTERRAIN_GRID { GRID_SIZE = 3 };

	class RZ_UTILITY_EXPORT SpaceManager
	{
	public:
		void InitSpaceManager(SpaceTravel* traveller, WeakGameObjectPtr owner, ComputeAPI* capi, Scene* scene);
		void Update(float deltaTime);
		void Render();

		void CreatePlanet(int32 seed, float x, float y, float z);

		void GenerateTerrain(int32 mipmap, int xCell, int yCell, int32 TerrainX, int32 TerrainY);
		void GenerateSurface();
		void ChangeCurrentPlanet(Planet* planet);

		void RegenerateTerrains();

		Planet* m_CurrentPlanet = NULL;

		float m_MinDist = 5000.0f;
		bool m_OnLand = false;

		int32 Octaves = 6;
		float Persistance = 0.5f;
		int32 Scale = 1;
		int32 VerticalScale = 5;
		int32 MipMap = 0;

		///Manager Pointers
		ResVault* m_res = NULL;
		Gfx::GfxCore* m_gfx = NULL;
		ComputeAPI* m_capi = NULL;
		Scene* m_scene = NULL;

		SpaceTravel* m_Traveller = NULL;

		void RenderSurface();
		void RenderSpace();
		int32 m_PlayerCubeSide;

		float surfaceScale = 1.0f;
		Planet* closestPlanet;

		bool Visible = true;

	private:
		void UpdateSpace(float deltaTime);
		void UpdateSurface(float deltaTime);

		void LandOnPlanet();
		void StitchTerrain(int32 x, int32 y);

		void CalculatePlayerPos();
		Vector3 m_PlayerCubePos;
		Vector2 m_PlayerCells;

		WeakGameObjectPtr m_Owner;

		//A vector that hold pointers to all the planets on this SpaceSystem.
		Vector<Planet*> m_Planets;
		
		// Loads and initializes all parts of the compute terrain generation.
		void InitCompute(ComputeAPI* compute);

		///The program containing all kernels used in terrain generation
		Program m_TerrainProgram;

		///The terrains a planet can use when rendered.
		MeshPlane* m_Terrains[3][3];

		///The Kernel for mesh generation
		Kernel m_MeshGenerator;
		///The Kernel for normals generation
		Kernel m_NormalGenerator;
		///The Kernel for smooth normal generation
		Kernel m_SmoothNormalGenerator;

		///The kernel for a patch's noise texture.
		Kernel m_NoiseTexGenerator;
		MemBuffer m_NoiseTexData;

		/////The kernel for a patch's noise texture.
		Kernel m_NormalTexGenerator;
		MemBuffer m_NoiseTexBuffer;
		MemBuffer m_NormalTexData;

		///The CL Memory Buffer for Indices
		MemBuffer m_CLIndicesBuffer;
		///The CL Memory Buffer for Vertices
		MemBuffer m_CLVerticesBuffer;
		
		///The index buffers for the different mip map levels.
		Gfx::IndexBuffer* m_IndexMipMaps;

		///The current position on the planet.
		Vector2 m_PlanetPosition;

		Vector2 m_OriginalPlanetPosition;

		float m_CurrentBorderX = 256;
		float m_CurrentBorderZ = 256;
	};
}