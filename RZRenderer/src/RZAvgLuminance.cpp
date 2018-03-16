#include <RZRendererPCH.h>

namespace rczEngine
{
	void AverageLuminancePass::SetRenderingMode(RENDERING_MODE mode)
	{
		m_gfx->CompileAndCreatePixelShader(m_AvgLuminanceShader, L"Shaders/HDRBloom/AvgLuminance.hlsl");
	}

	void AverageLuminancePass::PreRenderPass()
	{
		m_AvgLuminanceShader.SetThisPixelShader(m_gfx);

		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();

		m_gfx->ClearRenderTargetView(0, 0, 0, 0, 0);
		m_gfx->ClearDepthTargetView();
	}

	void AverageLuminancePass::RenderPass()
	{
		RacrozRenderer::RenderScreenAlignedQuad();
	}

	void AverageLuminancePass::PostRenderPass()
	{
		for (int32 i = 0; i < MAX_TEXTURES_PASS; ++i)
			m_gfx->UnbindPSShaderResource(i);

		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();

		if (m_BlendState)
			m_gfx->SetBlendStateDefault();
	}
}