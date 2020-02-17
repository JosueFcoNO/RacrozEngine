#include <RZRendererPCH.h>

namespace rczEngine
{
	void PBR_Pass::SetRenderingMode(eRenderingPipelines mode)
	{
		///Load the LUT.
		m_LUT = std::make_shared<Texture2D>();
		m_LUT->Load("RacrozEngineAssets/BRDF LUT.png", "LUT");

		m_PShaderPath = L"Shaders/PBR_Deferred.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		AddTexture2D(m_LUT, 7);

		config.AmbientLightIntensity = 1.0f;
		config.LightIntensity = 1.0f;
		config.ReflectionIntensity = 1.0f;
		config.Wireframe = false;
		config.R = config.G = config.B = 1;

		UserDisney.CreateConstantBuffer(sizeof(RendererConfig), Gfx::USAGE_DEFAULT, m_gfx);
		UserDisney.UpdateConstantBuffer(&config, m_gfx);
		UserDisney.SetBufferInPS(6, m_gfx);

		UseDepth = false;
	}

	void PBR_Pass::PreRenderPass()
	{
		LightManager::Pointer()->SetLightsBuffers();

		m_PShader.SetThisPixelShader(m_gfx);

		UserDisney.UpdateConstantBuffer(&config, m_gfx);
		UserDisney.SetBufferInPS(6, m_gfx);

		//m_gfx->ClearDepthTargetView();

		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();
	}
	
	void PBR_Pass::RenderPass()
	{
		RacrozRenderer::RenderScreenAlignedQuad();
	}

	void PBR_Pass::PostRenderPass()
	{
		m_gfx->UnbindRenderTargets();
		for (int32 i = 0; i < 10; ++i)
			m_gfx->UnbindPSShaderResource(i);

		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();

		if (m_BlendState)
			m_gfx->SetBlendStateDefault();
	}
};