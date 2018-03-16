#include "RZRendererPCH.h"

namespace rczEngine
{
	void Pass::AddRasterizerState(Gfx::RasterizerState * rasterizerState)
	{	
		m_RasterizerState = rasterizerState;
	}

	void Pass::AddBlendState(Gfx::BlendState * blendState, int32 slot)
	{
		if (m_BlendState)
			m_gfx->SetBlendState(*m_BlendState);
	}

	void Pass::AddTexture2D(Texture2D * texture, int32 slot)
	{
		m_Texture2D[slot] = texture;
	}

	void Pass::AddRenderTarget(Gfx::RenderTarget * renderTarget, int32 slot)
	{
		m_RenderTargets[slot] = renderTarget;
	}

	void Pass::SetRasterizerState()
	{
		if (m_RasterizerState)
		m_gfx->SetRSState(*m_RasterizerState);
	}

	void Pass::SetTexturesInPipeline()
	{
		for (int32 i = 0; i < MAX_TEXTURES_PASS; ++i)
		{
			if(m_Texture2D[i] != NULL)
			m_Texture2D[i]->SetThisTextureInPS(i, 1, m_gfx);
		}
	}

	void Pass::SetRenderTargetsInPipeline()
	{
		int32 i;
		for (i = 0; i < MAX_RENDER_TARGETS; ++i)
		{
			if (m_RenderTargets[i] != NULL)
			{
				m_gfx->AddRenderTarget(*m_RenderTargets[i], i);
			}
			else
			{
				break;
			}
		}

		if (m_RenderTargets[0] != NULL)
		{
			m_gfx->SetNumberOfRenderTargets(i);
			m_gfx->SetRenderTargets();
		}
	}


}