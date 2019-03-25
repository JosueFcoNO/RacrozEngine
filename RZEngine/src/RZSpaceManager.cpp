#include <RZEnginePCH.h>

namespace rczEngine
{
	SpaceManager*& SpaceManager::_Instance()
	{
		static SpaceManager* instance = nullptr;
		return instance;
	}

	void SpaceManager::Start()
	{
		(_Instance()) = new SpaceManager;
	}

	SpaceManager* SpaceManager::Pointer()
	{
		return _Instance();
	}

	void SpaceManager::ShutDown()
	{
		delete _Instance();
	}

	void SpaceManager::InitSpaceManager()
	{
		m_capi = ComputeAPI::Pointer();
		m_gfx = Gfx::GfxCore::Pointer();
		m_res = ResVault::Pointer();
		m_scene = SceneManager::Pointer()->GetActiveScene();
		//m_Traveller = NULL; //TODO: Arreglar esto
		//m_Owner = owner;

		surfaceScale = 100.0f;

		CreatePlanet(123123, 0, 0, 00.0f);
		m_CurrentPlanet = m_Planets[0];

		m_AtmosValues.CreateConstantBuffer(sizeof(AtmosData), Gfx::USAGE_DEFAULT, m_gfx);

		m_OnLand = false;

	}

	void SpaceManager::Update(float deltaTime)
	{
		m_CurrentPlanet = m_Planets[0];

		m_OnLand ? UpdateSurface(deltaTime) : UpdateSpace(deltaTime);
	}

	void SpaceManager::Render()
	{
		m_OnLand ? RenderSurface() : RenderSpace();
	}

	void SpaceManager::RenderAtmos()
	{
		for (int32 i = 0; i < m_Planets.size(); ++i)
		{
			m_Planets[i]->RenderAtmosphere();
		}
	}

	void SpaceManager::CreatePlanet(int32 seed, float x, float y, float z)
	{
		Planet* planet = new Planet;
		m_Planets.push_back(planet);

		planet->InitPlanet(seed, x, y, z, this);

		m_CurrentPlanet = planet;
	}

	///////////////////////////////////////
	///////////SPACE///////////////////////
	///////////////////////////////////////

	void SpaceManager::UpdateSpace(float deltaTime)
	{
		//Vector3 TravellerVector;
		//float minDist = 5000.0f;
		//
		//for (int32 i = 0; i < m_Planets.size(); ++i)
		//{
		//	TravellerVector = m_Traveller->GetOwner().lock()->GetPosition() - m_Planets[i]->GetSpacePosition();
		//
		//	float Distance = TravellerVector.Magnitude();
		//
		//	if (Distance < minDist)
		//	{
		//		closestPlanet = m_Planets[i];
		//		minDist = Distance;
		//	}
		//}

		//closestPlanet = m_Planets[0];

		if (closestPlanet && !m_OnLand)
		{
			ChangeCurrentPlanet(closestPlanet);
			//m_MinDist = minDist - RADIO;
			//
			//if (m_MinDist < RADIO_ATMOSFERA)
			//{
			//	m_PlanetPosition = Math::PolarToPos2D(TravellerVector.GetNormalized());
			//	m_PlanetPosition.m_x *= MAX_GRAD_X;
			//	m_PlanetPosition.m_y *= MAX_GRAD_Y;
			//
			//	m_PlanetPosition.m_x = floor(m_PlanetPosition.m_x);
			//	m_PlanetPosition.m_y = floor(m_PlanetPosition.m_y);
			//
			//	LandOnPlanet();
			//
			//	m_MinDist = 1.0f;
			//}
		}
	}

	void SpaceManager::RenderSpace()
	{
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
		//auto pos = m_Traveller->m_Camera->GetPosition();
		//auto xPlayer = m_PlanetPosition.m_x;
		//auto yPlayer = m_PlanetPosition.m_y;
		//
		//auto TravellerVector = m_Traveller->GetOwner().lock()->GetPosition() - closestPlanet->GetSpacePosition();
		//
		//float Distance = Math::Max(TravellerVector.Magnitude() - RADIO / 2.0f, 0.0f);
		//static float scale = 60.0f;
		//if (Distance <= RADIO_ATMOSFERA)
		//{
		//	surfaceScale = Math::Clamp(RADIO_ATMOSFERA*scale - (Distance)*scale + 1.0f, 1.0f, RADIO_ATMOSFERA*scale);
		//}
		//else
		{
			surfaceScale = 1.0f;
		}

		//m_Owner.lock()->SetPosition(m_CurrentPlanet->GetSpacePosition());
		//m_Owner.lock()->SetScale(surfaceScale, surfaceScale, surfaceScale);

		//m_Owner.lock()->UpdateWorldMatrix();
	}

	void SpaceManager::CalculatePlayerPos()
	{
		///Get the unit vector from the center of the planet to the player.
		//m_PlayerCubePos = m_Traveller->GetOwner().lock()->GetPosition().GetNormalized();
		/////Save the position of the player.
		//Vector3 poss = Math::SphereToCubePoint(m_PlayerCubePos);
		//
		//m_PlayerCubePos = poss * 25.0f;
		//m_PlayerCubePos.m_x = floorf(m_PlayerCubePos.m_x) + 25;
		//m_PlayerCubePos.m_y = floorf(m_PlayerCubePos.m_y) + 25;
		//m_PlayerCubePos.m_z = floorf(m_PlayerCubePos.m_z) + 25;
		//
		//if (poss.m_y >= 1.0f) //Y+
		//{
		//	m_PlayerCubeSide = 0;
		//	m_PlayerCells.m_x = m_PlayerCubePos.m_x;
		//	m_PlayerCells.m_y = m_PlayerCubePos.m_z;
		//}
		//else if (poss.m_x >= 1.0f) //X+
		//{
		//	m_PlayerCubeSide = 1;
		//	m_PlayerCells.m_x = m_PlayerCubePos.m_z;
		//	m_PlayerCells.m_y = m_PlayerCubePos.m_y;
		//}
		//else if (poss.m_z >= 1.0f) //Z+
		//{
		//	m_PlayerCubeSide = 2;
		//	m_PlayerCells.m_x = 50.0f - m_PlayerCubePos.m_x;
		//	m_PlayerCells.m_y = m_PlayerCubePos.m_y;
		//}
		//else if (poss.m_x <= -1.0f) //X-
		//{
		//	m_PlayerCubeSide = 3;
		//	m_PlayerCells.m_x = 50.0f - m_PlayerCubePos.m_z;
		//	m_PlayerCells.m_y = m_PlayerCubePos.m_y;
		//}
		//else if (poss.m_z <= -1.0f) //Z-
		//{
		//	m_PlayerCubeSide = 4;
		//	m_PlayerCells.m_x = m_PlayerCubePos.m_x;
		//	m_PlayerCells.m_y = m_PlayerCubePos.m_y;
		//}
		//else if (poss.m_y <= -1.0f) //Y-
		//{
		//	m_PlayerCubeSide = 5;
		//	m_PlayerCells.m_x = m_PlayerCubePos.m_x;
		//	m_PlayerCells.m_y = 50.0f - m_PlayerCubePos.m_z;
		//}

	}

	void SpaceManager::RenderSurface()
	{
		if (m_CurrentPlanet && m_OnLand)
		{
			auto ScaleMatrix = Matrix4::Translate3D(m_CurrentPlanet->GetSpacePosition().m_x, m_CurrentPlanet->GetSpacePosition().m_y, m_CurrentPlanet->GetSpacePosition().m_z)*Matrix4::Scale3D(RADIO * surfaceScale, RADIO * surfaceScale, RADIO * surfaceScale);

			m_scene->m_WorldMatrix.UpdateConstantBuffer(&ScaleMatrix.GetTransposed(), m_gfx);
			m_scene->m_WorldMatrix.SetBufferInVS(2, m_gfx);
			m_scene->m_WorldMatrix.SetBufferInHS(1, m_gfx);
		}
	}

	void SpaceManager::LandOnPlanet()
	{
		m_OnLand = true;

		//auto owner = m_Traveller->GetOwner();
	}

	void SpaceManager::ChangeCurrentPlanet(Planet * planet)
	{
		m_CurrentPlanet = planet;
	}

};