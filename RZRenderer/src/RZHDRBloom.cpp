#include <RZRendererPCH.h>

namespace rczEngine
{
	void HDRBloomPass::SetRenderingMode(RENDERING_MODE mode)
	{
		m_gfx->CompileAndCreatePixelShader(m_HDRBloomShader, L"Shaders/HDRBloom/HDRBloom.hlsl");
	}

	void HDRBloomPass::PreRenderPass()
	{
		m_HDRBloomShader.SetThisPixelShader(m_gfx);

		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();

		m_gfx->ClearRenderTargetView(0, 0, 0, 0, 0);
		m_gfx->ClearDepthTargetView();
	}

	void HDRBloomPass::RenderPass()
	{
		RacrozRenderer::RenderScreenAlignedQuad();
	}

	void HDRBloomPass::PostRenderPass()
	{
		for (int32 i = 0; i < MAX_TEXTURES_PASS; ++i)
			m_gfx->UnbindPSShaderResource(i);

		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();

		if (m_BlendState)
			m_gfx->SetBlendStateDefault();
	}
}