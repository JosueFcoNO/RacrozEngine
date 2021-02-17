#include <RZRendererPCH.h>

namespace rczEngine
{
	void ShadowPass::CreatePipeline(eRenderingPipelines mode)
	{
		m_PShaderPath = L"Shaders/ShadowPasse.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

	}

	void ShadowPass::PreRenderPass()
	{
		m_gfx->UnbindRenderTargets();

		LightManager::Pointer()->SetLightsBuffers();

		m_PShader.SetThisPixelShader(m_gfx);
		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();

		m_gfx->ClearRenderTargetView(0, 1, 1, 1, 1);
		m_gfx->ClearDepthTargetView();

		auto light = LightManager::Pointer()->GetLight(0);

		if (light)
		{
			LightManager::Pointer()->SetShadowLightBuffer(0);
		}
	}

	void ShadowPass::RenderPass()
	{
		RacrozRenderer::Pointer()->DrawScreenQuad();
	}

	void ShadowPass::PostRenderPass()
	{
		m_gfx->UnbindRenderTargets();
		for (int32 i = 0; i < MAX_TEXTURES_PASS; ++i)
			m_gfx->UnbindPSShaderResource(i);
		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();
		if (m_BlendState)
			m_gfx->SetBlendStateDefault();

	}
};