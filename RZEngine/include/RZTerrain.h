#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Terrain
	{
	public:
		void InitTerrain(Gfx::GfxCore* gfx, ResVault* res, ComputeAPI* Capi);
		void GenerateTerrain();
		void RenderTerrain();

		MeshPlane m_Mesh;

	private:
		PerlinNoise m_Perlin;

		Gfx::GfxCore* m_gfx;
		ResVault* m_res;
		ComputeAPI* m_Capi;
	};
};