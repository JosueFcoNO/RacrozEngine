#include <RZEnginePCH.h>

namespace rczEngine
{
	void HeightMap::LoadHeightMap(ResVault * res)
	{
	}

	void HeightMap::SetHeightMapInPipeline(Gfx::GfxCore * gfx, ResVault* res)
	{
		res->GetResource<Texture2D>(m_HeightMapTexture).lock()->SetThisTextureInVS(11, 1, gfx);
	}
}