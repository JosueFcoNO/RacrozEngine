#pragma once

namespace rczEngine
{
	class RZ_EXP HeightMap
	{
	public:
		void LoadHeightMap(ResVault * res);
		void SetHeightMapInPipeline(Gfx::GfxCore* gfx, ResVault* res);
		ResourceHandle GetHeightMap() { return m_HeightMapTexture; };

		ResourceHandle m_HeightMapTexture = INVALID_RESOURCE;
	protected:

	};

}