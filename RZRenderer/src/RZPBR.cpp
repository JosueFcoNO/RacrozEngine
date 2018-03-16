#include <RZRendererPCH.h>

namespace rczEngine
{
	void PBR_Pass::SetRenderingMode(RENDERING_MODE mode)
	{
		///Load the LUT.
		m_LUT = std::make_shared<Texture2D>();
		m_LUT->Load("RacrozEngineAssets/BRDF LUT.png", "LUT", false);

		m_gfx->CompileAndCreatePixelShader(m_PBRShader, L"Shaders/PBRGeneral.hlsl");

		AddTexture2D(m_LUT.get(), 7);

		config.AmbientLightIntensity = 1.0f;
		config.LightIntensity = 1.0f;
		config.ReflectionIntensity = 1.0f;
		config.Metallic = 0.5f;
		config.OverrideAlbedo = 0.0f;
		config.OverrideMetallic = 0.0f;
		config.OverrideNormal = 0.0f;
		config.OverrideRoughness = 0.0f;
		config.OverrideSpecular = 0.0f;
		config.Roughness = 0.5f;
		config.UserAlbedo.Set(0.0f, 0.0f, 0.0f, 0.0f);
		config.UserSpecular.Set(0.0f, 0.0f, 0.0f);
		config.Wireframe = false;
		config.TesselationFactor = 1;
		config.RenderTex = 0;
		config.R = config.G = config.B = config.A = 1;

		UserDisney.CreateConstantBuffer(sizeof(UserConfig), Gfx::USAGE_DEFAULT, m_gfx);
		UserDisney.UpdateConstantBuffer(&config, m_gfx);
		UserDisney.SetBufferInPS(6, m_gfx);
	}

	void PBR_Pass::PreRenderPass()
	{
		LightManager::Pointer()->SetLightsBuffers();

		m_PBRShader.SetThisPixelShader(m_gfx);

		UserDisney.UpdateConstantBuffer(&config, m_gfx);
		UserDisney.SetBufferInPS(6, m_gfx);

		m_gfx->ClearDepthTargetView();

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