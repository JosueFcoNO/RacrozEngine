#include <RZRendererPCH.h>

namespace rczEngine
{
	void BloomPass::SetRenderingMode(RENDERING_MODE mode)
	{
		m_PShaderPath = L"Shaders/HDRBloom/Bloom.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		m_BlurredBloomRT = RacrozRenderer::Pointer()->CreateRenderTargetAndTexture_XYScales("BlurredBloom", m_BlurredBloom, 2, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);
		AddTexture2D(m_BlurredBloom, 1);

		m_BloomPasses = 0;
	}

	void BloomPass::PreRenderPass()
	{
		if (m_BloomPasses++ == 0)
		{
			RacrozRenderer::Pointer()->DoBlurPass(m_BlurredBloomRT, m_OriginalBright, 1920/2, 1080/2);
		}
		else
		{
			RacrozRenderer::Pointer()->DoBlurPass(m_BlurredBloomRT, m_LastBloomResult, 1920/2, 1080/2);
		}

		m_BloomPasses %= 4;

		m_PShader.SetThisPixelShader(m_gfx);
		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();
		m_gfx->ClearRenderTargetView(0, 0, 0, 0, 0);
		m_gfx->ClearDepthTargetView();
	}

	void BloomPass::RenderPass()
	{
		RacrozRenderer::RenderScreenAlignedQuad();
	}

	void BloomPass::PostRenderPass()
	{
		for (int32 i = 0; i < MAX_TEXTURES_PASS; ++i)
			m_gfx->UnbindPSShaderResource(i);
		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();
		if (m_BlendState)
			m_gfx->SetBlendStateDefault();
	}

}