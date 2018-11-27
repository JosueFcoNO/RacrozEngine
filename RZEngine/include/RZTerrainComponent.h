#pragma once

namespace rczEngine
{
	class RZ_EXP TerrainRenderer : public Component
	{
	public:
		TerrainRenderer() {};

		void Init() {};
		void Update(float deltaTime) {};
		void Render(Gfx::GfxCore* gfx, ResVault* res, Scene* scene)
		{
			m_Terrain.RenderTerrain();
		};

		static const ComponentType s_ComponentType = CMP_TERRAIN_RENDERER;
		virtual ComponentType GetComponentType() { return TerrainRenderer::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		Terrain m_Terrain;
	};
}