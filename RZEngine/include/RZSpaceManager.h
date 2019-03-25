#pragma once

#define TERRAIN_NUMBER 13
#define RADIO 70.0f
#define RADIO_ATMOSFERA 2.5f

namespace rczEngine
{
	class RZ_EXP SpaceManager
	{
	private:
		static SpaceManager*& _Instance();

	public:
		static void Start();
		static SpaceManager* Pointer();
		static void ShutDown();

		void InitSpaceManager();
		void Update(float deltaTime);
		void Render();
		void RenderAtmos();

		void CreatePlanet(int32 seed, float x, float y, float z);
		void ChangeCurrentPlanet(Planet* planet);

		Planet* m_CurrentPlanet = NULL;

		float m_MinDist = 5000.0f;
		bool m_OnLand = false;

		int32 Octaves = 6;
		float Persistance = 0.5f;
		int32 Scale = 1;
		int32 VerticalScale = 5;
		int32 MipMap = 0;

		//SpaceTravel* m_Traveller = NULL;

		int32 m_PlayerCubeSide;

		float surfaceScale = 1.0f;
		Planet* closestPlanet;

		bool Visible = true;

		Gfx::ConstantBuffer m_AtmosValues;

	private:
		void RenderSurface();
		void RenderSpace();

		void UpdateSpace(float deltaTime);
		void UpdateSurface(float deltaTime);

		void LandOnPlanet();

		void CalculatePlayerPos();
		Vector3 m_PlayerCubePos;
		Vector2 m_PlayerCells;

		GameObject* m_Owner;

		///Manager Pointers
		ResVault* m_res = NULL;
		Gfx::GfxCore* m_gfx = NULL;
		ComputeAPI* m_capi = NULL;
		StrPtr<Scene> m_scene = NULL;

		//A vector that hold pointers to all the planets on this SpaceSystem.
		Vector<Planet*> m_Planets;
		
		///The current position on the planet.
		Vector2 m_PlanetPosition;
		Vector2 m_OriginalPlanetPosition;
	};
}