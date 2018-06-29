#pragma once
#define PERMUTATION_TABLE_SIZE 256

namespace rczEngine
{
	class RZ_UTILITY_EXPORT SpaceManager;

	class RZ_UTILITY_EXPORT PlanetVertex
	{
	public:
		Vector3 VertexPosition;
		Vector2 TextureCoordinates;
		Vector3 VertexNormals;
		Vector3 Tangents;
		Vector3 BiNormals;
		int Gradient[4];
	};

	class RZ_UTILITY_EXPORT Planet
	{
	public:
		void InitPlanet(int32 seed, float x, float y, float z, SpaceManager* spaceMng);
		void RenderPlanet(float scale);
		void RenderPlanetWater(float scale);
		void RenderAtmosphere(float scale);

		void CreateMaterial();

		FORCEINLINE Vector3 GetSpacePosition() { return m_SpacePosition; };

		Vector2 m_PolarPos;

		float m_PlanetNorm = 1.0f;
		float m_PlanetScale = 400.0f;
		float m_SpaceDist = 0.0f;
		float m_RealDist = 0.0f;

		ResourceHandle m_Materials;
		ResourceHandle m_HeightMap;
		Vector4 m_HeightScale;

	private:
		void LoadAndProcessModel();

		bool m_OnLand = false;

		StrPtr<Model> m_Planet;

		SpaceManager* m_SpaceMng;
		Gfx::GfxCore* m_GfxCore;
		StrPtr<Scene> m_CurrentScene;

		Gfx::ConstantBuffer m_GradientCB;
		Gfx::ConstantBuffer m_GradientsReal;
		Gfx::ConstantBuffer m_ScaleCB;

		Gfx::ConstantBuffer m_PosView;
		Vector4 PlanetPos;

		Gfx::ConstantBuffer m_HeightCameracb;
		Vector4 m_HeightCamera;
		
		Vector4 PermutationTable[PERMUTATION_TABLE_SIZE];

		Vector3 m_SpacePosition;

		Matrix4 m_PlanetMatrix;

		ResourceHandle Water;
	};
}