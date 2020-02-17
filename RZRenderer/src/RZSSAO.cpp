#include <RZRendererPCH.h>

namespace rczEngine
{
	void SSAOPass::SetRenderingMode(eRenderingPipelines mode)
	{
		m_PShaderPath = L"Shaders/SSAOPS.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		settingsCB.CreateConstantBuffer(sizeof(SSAOsettings), Gfx::USAGE_DEFAULT, Gfx::GfxCore::Pointer());
	}

	void SSAOPass::PreRenderPass()
	{
		UseDepth = false;
		m_PShader.SetThisPixelShader(m_gfx);
		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();
		
		settingsCB.UpdateConstantBuffer(&settings, m_gfx);
		settingsCB.SetBufferInPS(10, m_gfx);
		
		m_gfx->ClearRenderTargetView(0, 0, 0, 0, 0);
		////m_gfx->ClearDepthTargetView();
	}

	void SSAOPass::RenderPass()
	{
		RacrozRenderer::RenderScreenAlignedQuad();
	}

	void SSAOPass::PostRenderPass()
	{
		for (int32 i = 0; i < MAX_TEXTURES_PASS; ++i)
			m_gfx->UnbindPSShaderResource(i);
		//if (m_RasterizerState)
		//	m_gfx->SetRSStateDefault();
		//if (m_BlendState)
		//	m_gfx->SetBlendStateDefault();

		//RacrozRenderer::Pointer()->DoBlurPass(m_RenderTargets[0], m_Texture2D[0], 1920 / 2, 1080 / 2);

	}
}