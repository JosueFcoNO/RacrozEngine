#include <RZRendererPCH.h>

namespace rczEngine
{
	void BrightPass::SetRenderingMode(RENDERING_MODE mode)
	{
		m_PShaderPath = L"Shaders/HDRBloom/Bright.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		
	}

	void BrightPass::PreRenderPass()
	{
		m_PShader.SetThisPixelShader(m_gfx);

		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();

		m_gfx->ClearRenderTargetView(0, 0, 0, 0, 0);
		m_gfx->ClearDepthTargetView();
	}

	void BrightPass::RenderPass()
	{
		RacrozRenderer::RenderScreenAlignedQuad();
	}

	void BrightPass::PostRenderPass()
	{
		for (int32 i = 0; i < MAX_TEXTURES_PASS; ++i)
			m_gfx->UnbindPSShaderResource(i);

		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();

		if (m_BlendState)
			m_gfx->SetBlendStateDefault();
	}
}