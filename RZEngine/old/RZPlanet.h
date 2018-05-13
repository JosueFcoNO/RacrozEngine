#pragma once
#define MAX_PLANET_SIZE 2000
#define MAX_PLANET_DIST 1000

namespace rczEngine
{
	class RZ_UTILITY_EXPORT SpaceManager;

	class RZ_UTILITY_EXPORT Planet
	{
	public:
		void InitPlanet(int32 seed, float x, float y, float z, SpaceManager* spaceMng);
		void RenderPlanet(float scale);

		void CreateMaterial();

		PerlinNoise m_Perlin;

		FORCEINLINE Vector3 GetSpacePosition() { return m_SpacePosition; };

		Vector2 m_PolarPos;

		float m_PlanetNorm = 1.0f;
		float m_PlanetScale = 400.0f;
		float m_SpaceDist = 0.0f;
		float m_RealDist = 0.0f;

		MemBuffer m_CLDataBuffer;
		MemBuffer m_CLGradBuffer;
		MemBuffer m_CLGradIndexBuffer;

		ResourceHandle m_Materials;

	private:
		bool m_OnLand = false;

		Model m_PlanetModel;
		SpaceManager* m_Space;

		Vector3 m_SpacePosition;

		
	};
}