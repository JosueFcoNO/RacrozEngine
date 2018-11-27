#include <RZEnginePCH.h>

namespace rczEngine
{
	void PerlinNoise2D::InitPerlinNoise(int32 seed)
	{
		m_Core.GradientVectors[0] = Vector2(-1, 1);
		m_Core.GradientVectors[1] = Vector2(-1, -1);
		m_Core.GradientVectors[2] = Vector2(1, 1);
		m_Core.GradientVectors[3] = Vector2(1, -1);

		ResetSeedAndGrads(seed);

		auto gfx = Gfx::GfxCore::Pointer();
	
		m_PerlinCBuffer.CreateConstantBuffer(sizeof(NoiseCore), Gfx::USAGE_DEFAULT, gfx);
		m_PerlinCBuffer.UpdateConstantBuffer(&m_Core, gfx);
	}

	void PerlinNoise2D::ResetSeedAndGrads(int32 seed)
	{
		m_rnd.SetRandomSeed(seed);

		for (int32 y = 0; y < MAX_GRAD_Y; ++y)
			for (int32 x = 0; x < MAX_GRAD_X; ++x)
			{
				char index = char((m_rnd.GetRandomNumberN() + 3.0f) / 6.0f * 4);
				index = Math::Clamp(index, (char)0, (char)3);

				m_Core.GradientGrid[y][x] = index;
			}
	}

	void PerlinNoise2D::SetNoiseCoreInPS(int32 slot)
	{
		m_PerlinCBuffer.SetBufferInPS(slot, Gfx::GfxCore::Pointer());
	}

	void PerlinNoise2D::SetNoiseCoreInVS(int32 slot)
	{
		m_PerlinCBuffer.SetBufferInVS(slot, Gfx::GfxCore::Pointer());
	}

	void PerlinNoise2D::SetNoiseCoreInDS(int32 slot)
	{
		m_PerlinCBuffer.SetBufferInDS(slot, Gfx::GfxCore::Pointer());
	}

	ResourceHandle PerlinNoise2D::GetPerlinNoiseAsTextureRGBA32FLOAT(int32 w, int32 h, int32 octaves, Gfx::GfxCore* gfx, ResVault* res)
	{
		Vector4 planetColor1(m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN());
		planetColor1.Normalize();

		Vector4 planetColor2(m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN());
		planetColor2.Normalize();

		Vector4 planetColor3(m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN());
		planetColor3.Normalize();

		Vector<Vector4> Result;

		for (int32 y = 0; y < h; ++y)
			for (int32 x = 0; x < w; ++x)
			{
				float noise = Math::Clamp01(OctaveNoise(float(x) / w * MAX_GRAD_X, float(y) / h * MAX_GRAD_Y, octaves, 1));
				Vector4 FinalColor = Math::Lerp(planetColor1, planetColor2, noise);
				Result.push_back(FinalColor);
			}

		StrPtr<Texture2D> tex = std::make_shared<Texture2D>();
		gfx->CreateTexture(&Result[0], w * sizeof(Vector4), 1, *tex->GetTextureCore(), w, h, Gfx::BIND_SHADER_RESOURCE, Gfx::FORMAT_R32G32B32A32_FLOAT, 1, Gfx::USAGE_DEFAULT, Gfx::CPU_DEFAULT);
		m_HeightMapTexture = res->InsertResource(tex);

		Result.clear();

		return m_HeightMapTexture;
	}

	ResourceHandle PerlinNoise2D::GetPerlinNoiseAsTextureR8UNORM(int32 w, int32 h, int32 octaves, Gfx::GfxCore * gfx, ResVault * res)
	{
		Vector4 planetColor1(m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN());
		planetColor1.Normalize();

		Vector4 planetColor2(m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN());
		planetColor2.Normalize();

		Vector4 planetColor3(m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN(), m_rnd.GetRandomNumberN());
		planetColor3.Normalize();

		Vector<UCHAR> Result;

		for (int32 y = 0; y < h; ++y)
			for (int32 x = 0; x < w; ++x)
			{
				float noise = Math::Clamp01(OctaveNoise(float(x) / w * MAX_GRAD_X, float(y) / h * MAX_GRAD_Y, octaves, 0.7f));
				//float noise2 = Math::Clamp01(OctaveNoise(float(x) / w * MAX_GRAD_X, float(y) / h * MAX_GRAD_Y, 2, 1));
				//Vector4 FinalColor = Math::Lerp(planetColor1, planetColor2, noise);

				Result.push_back(Math::Clamp01(noise)*255);

			}

		StrPtr<Texture2D> tex = std::make_shared<Texture2D>();
		gfx->CreateTexture(&Result[0], w * sizeof(unsigned char), 1, *tex->GetTextureCore(), w, h, Gfx::BIND_SHADER_RESOURCE, Gfx::FORMAT_R8_UNORM, 1, Gfx::USAGE_DEFAULT, Gfx::CPU_DEFAULT);
		m_HeightMapTexture = res->InsertResource(tex);

		return m_HeightMapTexture;
	}

	float PerlinNoise2D::Noise(float x, float y)
	{
		int xi = ((int)x) % MAX_GRAD_X;
		int yi = ((int)y) % MAX_GRAD_Y;
		int xi2 = (xi + 1) % MAX_GRAD_X;
		int yi2 = (yi + 1) % MAX_GRAD_Y;

		auto gTopLeft = m_Core.GradientVectors[m_Core.GradientGrid[yi][xi]];
		auto gTopRight = m_Core.GradientVectors[m_Core.GradientGrid[yi][xi2]];
		auto gBottomLeft = m_Core.GradientVectors[m_Core.GradientGrid[yi2][xi]];
		auto gBottomRight = m_Core.GradientVectors[m_Core.GradientGrid[yi2][xi2]];

		float xf = x - (int)x;
		float yf = y - (int)y;
		float u = Fade(xf);
		float v = Fade(yf);

		xf *= 2.0f;
		yf *= 2.0f;
		xf -= 1.0f;
		yf -= 1.0f;

		Vector2 vTopLeft(xf, yf);
		Vector2 vTopRight(xf, yf);
		Vector2 vBottomLeft(xf, yf);
		Vector2 vBottomRight(xf, yf);

		vTopLeft = Vector2(-1, -1) - vTopLeft;
		vTopRight = Vector2(1, -1) - vTopRight;
		vBottomLeft = Vector2(-1, 1) - vBottomLeft;
		vBottomRight = Vector2(1, 1) - vBottomRight;

		float x1, x2;

		x1 = Math::Lerp(vTopLeft | gTopLeft,
			vTopRight | gTopRight,
			u);

		x2 = Math::Lerp(vBottomLeft | gBottomLeft,
			vBottomRight | gBottomRight,
			u);

		return Math::Lerp(x1, x2, v);
	}

	float PerlinNoise2D::OctaveNoise(float x, float y, int octaves, double persistence)
	{
		float total = 0;
		float frequency = 1;
		float amplitude = 1;
		float maxValue = 0;

		for (int i = 0; i < octaves; i++) {
			total += Noise(x * frequency, y * frequency) * amplitude;

			maxValue += amplitude;

			amplitude *= CastStatic<float>(persistence);
			frequency *= 2;
		}

		return total / maxValue;

	}
}
