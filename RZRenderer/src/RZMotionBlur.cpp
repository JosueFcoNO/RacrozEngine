#include <RZRendererPCH.h>

namespace rczEngine
{
	void MotionBlurPass::SetRenderingMode(RENDERING_MODE mode)
	{
		m_gfx->CompileAndCreatePixelShader(m_BlurShader, L"Shaders/MotionBlur.hlsl");
	}

	void MotionBlurPass::PreRenderPass()
	{
		m_BlurShader.SetThisPixelShader(m_gfx);
		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();
		m_gfx->ClearRenderTargetView(0, 0, 0, 0, 0);
		m_gfx->ClearDepthTargetView();
	}

	void MotionBlurPass::RenderPass()
	{
		RacrozRenderer::RenderScreenAlignedQuad();
	}

	void MotionBlurPass::PostRenderPass()
	{
		for (int32 i = 0; i < MAX_TEXTURES_PASS; ++i)
			m_gfx->UnbindPSShaderResource(i);
		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();
		if (m_BlendState)
			m_gfx->SetBlendStateDefault();
	}
}