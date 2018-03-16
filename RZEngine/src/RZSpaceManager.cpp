#include <RZEnginePCH.h>

namespace rczEngine
{
	void SpaceManager::InitSpaceManager(SpaceTravel* traveller, WeakGameObjectPtr owner, ComputeAPI * capi, Scene * scene)
	{
		m_capi = capi;
		m_gfx = Gfx::GfxCore::Pointer();
		m_res = ResVault::Pointer();
		m_scene = scene;
		m_Traveller = traveller;
		m_Owner = owner;

		///Init and generate the terrains.
		for (int i = 0; i < 3; ++i)
			for (int k = 0; k < 3; ++k)
			{
				m_Terrains[i][k] = new MeshPlane;
				m_Terrains[i][k]->InitMeshPlane(32, 512.0f, capi, false);
			}

		///Allocate and init the 5 index buffers, one for each mip map level.
		m_IndexMipMaps = new Gfx::IndexBuffer;
		MeshPlane::GenerateIndices(32, *m_IndexMipMaps);
		auto indexSize = m_IndexMipMaps->GetSize();

		InitCompute(capi);

		m_CLIndicesBuffer.CreateMemBuffer(indexSize * sizeof(uint32), MEM_READ_ONLY, capi);
		m_CLIndicesBuffer.EnqueueWriteBuffer(indexSize * sizeof(uint32), m_IndexMipMaps->GetVectorStart(), m_capi);

	}

	void SpaceManager::Update(float deltaTime)
	{
		m_OnLand ? UpdateSurface(deltaTime) : UpdateSpace(deltaTime);
	}

	void SpaceManager::Render()
	{
		m_OnLand ? RenderSurface() : RenderSpace();
	}

	void SpaceManager::CreatePlanet(int32 seed, float x, float y, float z)
	{
		Planet* planet = new Planet;
		m_Planets.push_back(planet);

		planet->InitPlanet(seed, x, y, z, this);
	}

	///////////////////////////////////////
	///////////SPACE///////////////////////
	///////////////////////////////////////

	void SpaceManager::UpdateSpace(float deltaTime)
	{
		Vector3 TravellerVector;
		float minDist = 5000.0f;

		for (int32 i = 0; i < m_Planets.size(); ++i)
		{
			TravellerVector = m_Traveller->GetOwner().lock()->GetPosition() - m_Planets[i]->GetSpacePosition();

			float Distance = TravellerVector.Magnitude();

			if (Distance < minDist)
			{
				closestPlanet = m_Planets[i];
				minDist = Distance;
			}
		}

		if (closestPlanet && !m_OnLand)
		{
			ChangeCurrentPlanet(closestPlanet);
			m_MinDist = minDist - RADIO;

			if (m_MinDist < RADIO_ATMOSFERA)
			{
				m_PlanetPosition = Math::PolarToPos2D(TravellerVector.GetNormalized());
				m_PlanetPosition.m_x *= MAX_GRAD_X;
				m_PlanetPosition.m_y *= MAX_GRAD_Y;

				m_PlanetPosition.m_x = floor(m_PlanetPosition.m_x);
				m_PlanetPosition.m_y = floor(m_PlanetPosition.m_y);

				LandOnPlanet();

				m_MinDist = 1.0f;
			}
		}
	}

	void SpaceManager::RenderSpace()
	{
		if (Visible)
			for (int32 i = 0; i < m_Planets.size(); ++i)
			{
				m_Planets[i]->RenderPlanet(surfaceScale);
			}
	}

	///////////////////////////////////////
	///////////SURFACE/////////////////////
	///////////////////////////////////////

	void SpaceManager::UpdateSurface(float deltaTime)
	{
		auto pos = m_Traveller->m_Camera->GetPosition();
		auto xPlayer = m_PlanetPosition.m_x;
		auto yPlayer = m_PlanetPosition.m_y;

		auto TravellerVector = m_Traveller->GetOwner().lock()->GetPosition() - closestPlanet->GetSpacePosition();

		float Distance = Math::Max(TravellerVector.Magnitude() - RADIO / 2.0f, 0.0f);
		static float scale = 60.0f;
		if (Distance <= RADIO_ATMOSFERA)
		{
			surfaceScale = Math::Clamp(RADIO_ATMOSFERA*scale - (Distance)*scale + 1.0f, 1.0f, RADIO_ATMOSFERA*scale);
		}
		else
		{
			surfaceScale = 1.0f;
		}

		m_Owner.lock()->SetPosition(m_CurrentPlanet->GetSpacePosition());
		m_Owner.lock()->SetScale(surfaceScale, surfaceScale, surfaceScale);

		m_Owner.lock()->UpdateWorldMatrix();

		auto lastPlayerPos = m_PlayerCells;
		CalculatePlayerPos();
		if (lastPlayerPos != m_PlayerCells)
		{
			GenerateSurface();
		};
	}

	void SpaceManager::CalculatePlayerPos()
	{
		///Get the unit vector from the center of the planet to the player.
		m_PlayerCubePos = m_Traveller->GetOwner().lock()->GetPosition().GetNormalized();
		///Save the position of the player.
		Vector3 poss = Math::SphereToCubePoint(m_PlayerCubePos);

		m_PlayerCubePos = poss * 25.0f;
		m_PlayerCubePos.m_x = floorf(m_PlayerCubePos.m_x) + 25;
		m_PlayerCubePos.m_y = floorf(m_PlayerCubePos.m_y) + 25;
		m_PlayerCubePos.m_z = floorf(m_PlayerCubePos.m_z) + 25;

		if (poss.m_y >= 1.0f) //Y+
		{
			m_PlayerCubeSide = 0;
			m_PlayerCells.m_x = m_PlayerCubePos.m_x;
			m_PlayerCells.m_y = m_PlayerCubePos.m_z;
		}
		else if (poss.m_x >= 1.0f) //X+
		{
			m_PlayerCubeSide = 1;
			m_PlayerCells.m_x = m_PlayerCubePos.m_z;
			m_PlayerCells.m_y = m_PlayerCubePos.m_y;
		}
		else if (poss.m_z >= 1.0f) //Z+
		{
			m_PlayerCubeSide = 2;
			m_PlayerCells.m_x = 50.0f - m_PlayerCubePos.m_x;
			m_PlayerCells.m_y = m_PlayerCubePos.m_y;
		}
		else if (poss.m_x <= -1.0f) //X-
		{
			m_PlayerCubeSide = 3;
			m_PlayerCells.m_x = 50.0f - m_PlayerCubePos.m_z;
			m_PlayerCells.m_y = m_PlayerCubePos.m_y;
		}
		else if (poss.m_z <= -1.0f) //Z-
		{
			m_PlayerCubeSide = 4;
			m_PlayerCells.m_x = m_PlayerCubePos.m_x;
			m_PlayerCells.m_y = m_PlayerCubePos.m_y;
		}
		else if (poss.m_y <= -1.0f) //Y-
		{
			m_PlayerCubeSide = 5;
			m_PlayerCells.m_x = m_PlayerCubePos.m_x;
			m_PlayerCells.m_y = 50.0f - m_PlayerCubePos.m_z;
		}

	}

	void SpaceManager::InitCompute(ComputeAPI * compute)
	{
		///Buffer to hold the 4 gradient vectors.
		m_CLVerticesBuffer.CreateMemBuffer(sizeof(Gfx::Vertex) * 32 * 32, eMEMORY_FLAGS::MEM_READ_WRITE, compute);

		///Create and compile the compute program.
		m_TerrainProgram.CreateProgramFromFile("TerrainProgram.cu", compute);
		compute->BuildProgram(m_TerrainProgram);

		///Mesh generating kernel
		m_MeshGenerator.CreateKernel("GenerateMesh", compute, &m_TerrainProgram);
		m_MeshGenerator.SetKernelArgument(m_CLVerticesBuffer, 3);

		///Normal generating kernel
		m_NormalGenerator.CreateKernel("GenerateNormals", compute, &m_TerrainProgram);
		m_NormalGenerator.SetKernelArgument(m_CLVerticesBuffer, 2);

		///Noise Texture generating kernel
		m_NoiseTexGenerator.CreateKernel("GenerateNoise", compute, &m_TerrainProgram);
		m_NoiseTexData.CreateMemBuffer(sizeof(float) * HEIGHTMAP_RES * HEIGHTMAP_RES, eMEMORY_FLAGS::MEM_READ_WRITE, compute);
		m_NoiseTexGenerator.SetKernelArgument(m_NoiseTexData, 3);

		///Normal generating kernel
		m_NormalTexGenerator.CreateKernel("GenerateNormalTexture", compute, &m_TerrainProgram);
		m_NormalTexData.CreateMemBuffer(sizeof(float) * HEIGHTMAP_RES * HEIGHTMAP_RES, eMEMORY_FLAGS::MEM_WRITE_ONLY, compute);
		m_NormalTexGenerator.SetKernelArgument(m_NoiseTexData, 0);
		m_NormalTexGenerator.SetKernelArgument(m_NormalTexData, 1);
	}

	void SpaceManager::RenderSurface()
	{
		if (m_CurrentPlanet && m_OnLand)
		{
			auto ScaleMatrix = Matrix4::Translate3D(m_CurrentPlanet->GetSpacePosition().m_x, m_CurrentPlanet->GetSpacePosition().m_y, m_CurrentPlanet->GetSpacePosition().m_z)*Matrix4::Scale3D(RADIO * surfaceScale, RADIO * surfaceScale, RADIO * surfaceScale);

			m_scene->m_WorldMatrix.UpdateConstantBuffer(&ScaleMatrix.GetTransposed(), m_gfx);
			m_scene->m_WorldMatrix.SetBufferInVS(2, m_gfx);
			m_scene->m_WorldMatrix.SetBufferInHS(1, m_gfx);

			m_IndexMipMaps->SetThisIndexBuffer(m_gfx);

			for (int32 i = 0; i < 3; ++i)
				for (int32 k = 0; k < 3; ++k)
				{
					m_Terrains[i][k]->RenderMeshPlane(m_IndexMipMaps->m_IndexSize);
				}
		}
	}

	void SpaceManager::LandOnPlanet()
	{
		m_OnLand = true;

		auto owner = m_Traveller->GetOwner();

		for (int32 i = 0; i < GRID_SIZE; ++i)
			for (int32 k = 0; k < GRID_SIZE; ++k)
			{
				m_Terrains[i][k]->SetMaterial(closestPlanet->m_Materials);
			}

		GenerateSurface();
		
		m_CurrentPlanet->m_Perlin.SetNoiseCoreInDS(10);
	}

	void SpaceManager::StitchTerrain(int32 x, int32 y)
	{
		//If the terrain was already stitched and updated, don't do anything.
		if (!m_Terrains[x][y]->m_Dirty) return;

		auto CurrentTerrain = m_Terrains[x][y];
		if (y > 0)
			for (int i = 0; i < HEIGHTMAP_RES; ++i)
			{
				auto average = CurrentTerrain->m_NoisePatch[0][i] + m_Terrains[x][y - 1]->m_NoisePatch[HEIGHTMAP_RES - 1][i];
				average /= 2.0f;

				CurrentTerrain->m_NoisePatch[0][i] = m_Terrains[x][y - 1]->m_NoisePatch[HEIGHTMAP_RES - 1][i] = average;
			}

		if (x > 0)
			for (int i = 0; i < HEIGHTMAP_RES; ++i)
			{
				auto averageX = CurrentTerrain->m_NoisePatch[i][0] + m_Terrains[x-1][y]->m_NoisePatch[i][HEIGHTMAP_RES - 1];
				averageX /= 2.0f;
				
				CurrentTerrain->m_NoisePatch[i][0] = m_Terrains[x - 1][y]->m_NoisePatch[i][HEIGHTMAP_RES - 1] = averageX;
			}
	}

	void SpaceManager::ChangeCurrentPlanet(Planet * planet)
	{
		m_CurrentPlanet = planet;

		m_MeshGenerator.SetKernelArgument(planet->m_CLDataBuffer, 0);
		m_MeshGenerator.SetKernelArgument(planet->m_CLGradIndexBuffer, 1);
		m_MeshGenerator.SetKernelArgument(planet->m_CLGradBuffer, 2);

		m_NormalGenerator.SetKernelArgument(planet->m_CLDataBuffer, 0);

		m_NoiseTexGenerator.SetKernelArgument(planet->m_CLDataBuffer, 0);
		m_NoiseTexGenerator.SetKernelArgument(planet->m_CLGradIndexBuffer, 1);
		m_NoiseTexGenerator.SetKernelArgument(planet->m_CLGradBuffer, 2);
	}

	void SpaceManager::RegenerateTerrains()
	{
		GenerateTerrain(4, 1, 0, 0, 0);
		GenerateTerrain(4, 0, 0, 1, 0);
		GenerateTerrain(4, -1, 0, 2, 0);

		GenerateTerrain(4, -1, 1, 0, 1);
		GenerateTerrain(4, 0, 1, 1, 1);
		GenerateTerrain(4, 1, 1, 2, 1);

		GenerateTerrain(4, -1, 2, 0, 2);
		GenerateTerrain(4, 0, 2, 1, 2);
		GenerateTerrain(4, 1, 2, 2, 2);
	}

	void SpaceManager::GenerateTerrain(int32 mipmap, int xCell, int yCell, int32 TerrainX, int32 TerrainY)
	{
		//m_Terrains[TerrainIndex].ChangeMipMap(mipmap);

		int32 size2 = m_Terrains[TerrainX][TerrainY]->m_MeshBuffer.Size;
		size2 *= size2;

		m_Terrains[TerrainX][TerrainY]->m_MeshBuffer.xCell = xCell;
		m_Terrains[TerrainX][TerrainY]->m_MeshBuffer.yCell = yCell;

		m_Terrains[TerrainX][TerrainY]->m_MeshBuffer.CubeSide = m_PlayerCubeSide;

		m_Terrains[TerrainX][TerrainY]->m_MeshBuffer.Octaves = Octaves;
		m_Terrains[TerrainX][TerrainY]->m_MeshBuffer.Persistance = Persistance;

		////////MESH GENERATION
		m_CurrentPlanet->m_CLDataBuffer.EnqueueWriteBuffer(sizeof(MeshPlaneBuffer), &m_Terrains[TerrainX][TerrainY]->m_MeshBuffer, m_capi);
		m_capi->RunKernel(m_MeshGenerator, size2, 64);

		////////NORMALS GENERATION
		auto indexSize = m_IndexMipMaps->GetSize();
		m_NormalGenerator.SetKernelArgument(m_CLIndicesBuffer, 1);
		m_capi->RunKernel(m_NormalGenerator, indexSize / 3, 64);

		///////UPDATE VERTEX BUFFER
		m_CLVerticesBuffer.EnqueueReadBuffer(size2 * sizeof(Gfx::Vertex), m_Terrains[TerrainX][TerrainY]->m_VertexBuffer.GetVectorPtr(), m_capi);

		m_Terrains[TerrainX][TerrainY]->m_VertexBuffer.GetVertex(0).Tangents = m_Terrains[TerrainX][TerrainY]->m_VertexBuffer.GetVertex(1).Tangents;
		m_Terrains[TerrainX][TerrainY]->m_VertexBuffer.GetVertex(0).VertexNormals = m_Terrains[TerrainX][TerrainY]->m_VertexBuffer.GetVertex(1).VertexNormals;
		m_Terrains[TerrainX][TerrainY]->m_VertexBuffer.GetVertex(0).BiNormals = m_Terrains[TerrainX][TerrainY]->m_VertexBuffer.GetVertex(1).BiNormals;

		m_capi->RunKernel(m_NoiseTexGenerator, HEIGHTMAP_RES * HEIGHTMAP_RES, 64);
		m_NoiseTexData.EnqueueReadBuffer(sizeof(float) * HEIGHTMAP_RES *HEIGHTMAP_RES, m_Terrains[TerrainX][TerrainY]->m_NoisePatch, m_capi);

		//m_capi->RunKernel(m_NormalTexGenerator, HEIGHTMAP_RES * HEIGHTMAP_RES, 64);
		//m_NormalTexData.EnqueueReadBuffer(sizeof(Vector3) * HEIGHTMAP_RES *HEIGHTMAP_RES, m_Terrains[TerrainX][TerrainY]->m_NormalTex, m_capi);

		m_Terrains[TerrainX][TerrainY]->m_Dirty = true;
	}

	void SpaceManager::GenerateSurface()
	{
		m_OriginalPlanetPosition = m_PlanetPosition;

		GenerateTerrain(4, m_PlayerCells.m_x - 1, m_PlayerCells.m_y - 1, 0, 0);
		GenerateTerrain(4, m_PlayerCells.m_x, m_PlayerCells.m_y - 1, 1, 0);
		GenerateTerrain(4, m_PlayerCells.m_x + 1, m_PlayerCells.m_y - 1, 2, 0);

		GenerateTerrain(4, m_PlayerCells.m_x - 1, m_PlayerCells.m_y, 0, 1);
		GenerateTerrain(4, m_PlayerCells.m_x, m_PlayerCells.m_y, 1, 1);
		GenerateTerrain(4, m_PlayerCells.m_x + 1, m_PlayerCells.m_y, 2, 1);

		GenerateTerrain(4, m_PlayerCells.m_x - 1, m_PlayerCells.m_y + 1, 0, 2);
		GenerateTerrain(4, m_PlayerCells.m_x, m_PlayerCells.m_y + 1, 1, 2);
		GenerateTerrain(4, m_PlayerCells.m_x + 1, m_PlayerCells.m_y + 1, 2, 2);

		StitchTerrain(0, 0);
		StitchTerrain(1, 0);
		StitchTerrain(2, 0);

		StitchTerrain(0, 1);
		StitchTerrain(1, 1);
		StitchTerrain(2, 1);

		StitchTerrain(0, 2);
		StitchTerrain(1, 2);
		StitchTerrain(2, 2);

		for (int i = 0; i < 3; ++i)
			for (int k = 0; k < 3; ++k)
			{
				m_gfx->UpdateSubResource(m_res->GetResource<Texture2D>(m_Terrains[i][k]->HeightMap).lock()->GetTextureCore(), m_Terrains[i][k]->m_NoisePatch, HEIGHTMAP_RES * sizeof(float), 1);
				m_gfx->UpdateSubResource(&m_Terrains[i][k]->m_VertexBuffer, m_Terrains[i][k]->m_VertexBuffer.GetVectorPtr(), 0, 0, 1024 * sizeof(Gfx::Vertex));
			}
	}

};