#pragma once

#define MAX_GRAD_X 64
#define MAX_GRAD_Y 64

namespace rczEngine
{
	struct NoiseCore
	{
		Vector2 GradientVectors[4];
		char GradientGrid[MAX_GRAD_Y][MAX_GRAD_X];
	};

	class RZ_UTILITY_EXPORT PerlinNoise : public HeightMap
	{
	public:
		void InitPerlinNoise(int32 seed);
		void ResetSeedAndGrads(int32 seed);
		void SetNoiseCoreInPS(int32 slot);
		void SetNoiseCoreInVS(int32 slot);
		void SetNoiseCoreInDS(int32 slot);

		ResourceHandle GetPerlinNoiseAsTextureRGBA32FLOAT(int32 w, int32 h, int32 octaves, Gfx::GfxCore* gfx, ResVault* res);
		ResourceHandle GetPerlinNoiseAsTextureR8UNORM(int32 w, int32 h, int32 octaves, Gfx::GfxCore* gfx, ResVault* res);

		float Noise(float x, float y);
		float OctaveNoise(float x, float y, int octaves, double persistence);
	
		NoiseCore m_Core;

	private:
		float Fade(float t)
		{
			return t * t * t * (t * (t * 6 - 15) + 10);
		};

		RandomBoxMuller m_rnd;
		Gfx::ConstantBuffer m_PerlinCBuffer;
	};
}