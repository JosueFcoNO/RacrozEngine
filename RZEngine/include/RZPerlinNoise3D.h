#pragma once

namespace rczEngine
{
	class RZ_EXP PerlinNoise3D
	{
	public:
		void InitPerlinNoise(int32 seed);

		float Noise(Vector3 pos);
		float Noise(float x, float y, float z);
		float OctaveNoise(Vector3 pos, int octaves, double persistence);
		float OctaveNoise(float x, float y, float z, int octaves, double persistence);
		float RidgedOctaveNoise(float x, float y, float z, int octaves, double persistence);
		float RidgedOctaveNoise(Vector3 pos, int octaves, double persistence);

	private:
		unsigned char PermutationTable[255];

		float Quintic(float t)
		{
			return t * t * t * (t * (t * 6 - 15) + 10);
		};

		int hash(const int x, const int y, const int z)
		{
			return (int)PermutationTable[(int)PermutationTable[(int)PermutationTable[x] + y] + z];
		}

		float gradientDotV(int perm,Vector3 vec)
		{
			switch (perm & 11) {
			case 0: return vec.m_x + vec.m_y; // (1,1,0)
			case 1: return -vec.m_x + vec.m_y; // (-1,1,0)
			case 2: return vec.m_x - vec.m_y; // (1,-1,0)
			case 3: return -vec.m_x - vec.m_y; // (-1,-1,0)
			case 4: return vec.m_x + vec.m_z; // (1,0,1)
			case 5: return -vec.m_x + vec.m_z; // (-1,0,1)
			case 6: return vec.m_x - vec.m_z; // (1,0,-1)
			case 7: return -vec.m_x - vec.m_z; // (-1,0,-1)
			case 8: return vec.m_y + vec.m_z; // (0,1,1),
			case 9: return -vec.m_y + vec.m_z; // (0,-1,1),
			case 10: return vec.m_y - vec.m_z; // (0,1,-1),
			case 11: return -vec.m_y - vec.m_z; // (0,-1,-1)
			}
		}

		RandomBoxMuller m_rnd;
	};
}