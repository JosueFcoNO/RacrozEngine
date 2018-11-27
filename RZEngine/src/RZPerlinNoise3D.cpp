#include <RZEnginePCH.h>

namespace rczEngine
{
	void PerlinNoise3D::InitPerlinNoise(int32 seed)
	{
		RandomBoxMuller rnd;
		rnd.SetRandomSeed(seed);

		for (int i = 0; i < PERMUTATION_TABLE_SIZE; ++i)
		{
			PermutationTable[i] = (unsigned char)floor(rnd.GetRandomNumberN() * 255);
		}
	}

	float PerlinNoise3D::Noise(Vector3 pos)
	{
		return Noise(pos.m_x, pos.m_y, pos.m_z);
	}

	float PerlinNoise3D::Noise(float x, float y, float z)
	{
		int icoordx = (int)x;
		int icoordy = (int)y;
		int icoordz = (int)z;

		int xLeft = icoordx & 255;
		int xRight = (icoordx + 1) & 255;

		int yBottom = (icoordy) & 255;
		int yTop = (icoordy + 1) & 255;

		int zFront = icoordz & 255;
		int zBack = (icoordz + 1) & 255;

		Vector3 Coord;

		Coord.m_x = x - icoordx;
		Coord.m_y = y - icoordy;
		Coord.m_z = z - icoordz;

		float u = Quintic(Coord.m_x);
		float v = Quintic(Coord.m_y);
		float w = Quintic(Coord.m_z);

		Vector3 NoisePos = (Coord * 2.0f) - Vector3(1.0f, 1.0f, 1.0f);

		Vector3 vFrontTopLeft = NoisePos - Vector3(-1.0f, 1.0f, -1.0f);
		Vector3 vFrontTopRight = NoisePos - Vector3(1.0f, 1.0f, -1.0f);
		Vector3 vFrontBottomLeft = NoisePos - Vector3(-1.0f, -1.0f, -1.0f);
		Vector3 vFrontBottomRight = NoisePos - Vector3(1.0f, -1.0f, -1.0f);

		Vector3 vBackTopLeft = NoisePos - Vector3(-1.0f, 1.0f, 1.0f);
		Vector3 vBackTopRight = NoisePos - Vector3(1.0f, 1.0f, 1.0f);
		Vector3 vBackBottomLeft = NoisePos - Vector3(-1.0f, -1.0f, 1.0f);
		Vector3 vBackBottomRight = NoisePos - Vector3(1.0f, -1.0f, 1.0f);

		float a = gradientDotV(hash(xLeft, yBottom, zFront), vFrontBottomLeft);
		float b = gradientDotV(hash(xRight, yBottom, zFront), vFrontBottomRight);
		float c = gradientDotV(hash(xLeft, yTop, zFront), vFrontTopLeft);
		float d = gradientDotV(hash(xRight, yTop, zFront), vFrontTopRight);

		float e = gradientDotV(hash(xLeft, yBottom, zBack), vBackBottomLeft);
		float f = gradientDotV(hash(xRight, yBottom, zBack), vBackBottomRight);
		float g = gradientDotV(hash(xLeft, yTop, zBack), vBackTopLeft);
		float h = gradientDotV(hash(xRight, yTop, zBack), vBackTopRight);

		return (Math::Lerp(
			Math::Lerp(Math::Lerp(a, b, u), Math::Lerp(c, d, u), v),
			Math::Lerp(Math::Lerp(e, f, u), Math::Lerp(g, h, u), v),
			w));
	}

	float PerlinNoise3D::OctaveNoise(Vector3 pos, int octaves, double persistence)
	{
		return OctaveNoise(pos.m_x, pos.m_y, pos.m_z, octaves, persistence);
	}

	float PerlinNoise3D::OctaveNoise(float x, float y, float z, int octaves, double persistence)
	{
		float total = 0;
		float frequency = 1;
		float amplitude = 1;
		float maxValue = 0;

		for (int i = 0; i < octaves; i++)
		{
			float current = Noise(Vector3(x,y,z) * frequency);
			total += current * amplitude;

			maxValue += amplitude;

			amplitude *= CastStatic<float>(persistence);
			frequency *= 2;
		}

		total /= maxValue;

		return total;
	}

	float PerlinNoise3D::RidgedOctaveNoise(float x, float y, float z, int octaves, double persistence)
	{
		float total = 0;
		float frequency = 1;
		float amplitude = 1;
		float maxValue = 0;

		for (int i = 0; i < octaves; i++)
		{
			float current = 1.0f - Math::fAbs(Noise(Vector3(x, y, z) * frequency));
			total += current * amplitude;

			maxValue += amplitude;

			amplitude *= CastStatic<float>(persistence);
			frequency *= 2;
		}

		total /= maxValue;

		return total;
	}

	float PerlinNoise3D::RidgedOctaveNoise(Vector3 pos, int octaves, double persistence)
	{
		return RidgedOctaveNoise(pos.m_x, pos.m_y, pos.m_z, octaves, persistence);
	}
}